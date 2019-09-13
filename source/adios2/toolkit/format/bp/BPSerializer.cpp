/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BPSerializer.cpp
 *
 *  Created on: Sep 9, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "BPSerializer.h"

namespace adios2
{
namespace format
{

BPSerializer::BPSerializer(const helper::Comm &comm, const bool debugMode)
: BPBase(comm, debugMode)
{
}

void BPSerializer::MakeHeader(Buffer &b, const std::string fileType,
                              const bool isActive)
{
}

void BPSerializer::PutProcessGroupIndex(
    const std::string &ioName, const std::string hostLanguage,
    const std::vector<std::string> &transportsTypes)
{
    m_Profiler.Start("buffering");
    BufferSTL &index = m_MetadataSet.PGIndex.Buffer;

    const size_t pgBeginPosition = m_Data->Position();
    const size_t pgAbsolutePosition = m_Data->AbsolutePosition();

    // write a block identifier opening [PGI
    const std::string pgi = "[PGI";
    m_Data->Insert(pgi.c_str(), pgi.size());
    m_MetadataSet.DataPGLengthPosition = m_Data->Position();
    m_Data->Insert('\0', 8); // skip data pg length (8 bytes)

    const std::size_t metadataPGLengthPosition = index.Position();
    index.Insert('\0', 2); // skip metadata pg length (2 bytes)
    // write io name to metadata
    InsertBPString(ioName, index);

    // write if data is column major in metadata and data
    const char columnMajor =
        (helper::IsRowMajor(hostLanguage) == false) ? 'y' : 'n';
    index.Insert(columnMajor);
    m_Data->Insert(columnMajor);

    // write io name in data
    InsertBPString(ioName, *m_Data);

    // processID in metadata,
    const uint32_t processID = static_cast<uint32_t>(m_RankMPI);
    index.Insert(processID);

    // skip coordination var in data ....what is coordination var?
    m_Data->Insert('\0', 4);

    // time step name to metadata and data
    const std::string timeStepName = std::to_string(m_MetadataSet.TimeStep);
    InsertBPString(timeStepName, index);
    InsertBPString(timeStepName, *m_Data);

    // time step to metadata and data
    index.Insert(m_MetadataSet.TimeStep);
    m_Data->Insert(m_MetadataSet.TimeStep);

    // offset to pg in data in metadata which is the current absolute position
    const uint64_t pgOffsetInData =
        static_cast<uint64_t>(pgAbsolutePosition + m_PreDataFileLength);
    index.Insert(pgOffsetInData);

    // Back to writing metadata pg index length (length of group)
    const uint16_t metadataPGIndexLength =
        static_cast<uint16_t>(index.Position() - metadataPGLengthPosition - 2);

    size_t backPosition = metadataPGLengthPosition;
    index.Insert(backPosition, metadataPGIndexLength);
    // DONE With index

    // here write method in data
    const std::vector<uint8_t> methodIDs = GetTransportIDs(transportsTypes);
    const uint8_t methodsCount = static_cast<uint8_t>(methodIDs.size());
    const uint16_t methodsLength = static_cast<uint16_t>(methodsCount * 3);
    m_Data->Insert(methodsCount);
    m_Data->Insert(methodsLength);

    for (const uint8_t methodID : methodIDs)
    {
        m_Data->Insert(methodID);
        m_Data->Insert('\0', 2); // skip method parameters (2 bytes)
    }

    // pg vars count and position
    m_MetadataSet.DataPGVarsCount = 0;
    m_MetadataSet.DataPGVarsCountPosition = m_Data->Position();
    // add vars count and length
    m_Data->Insert('\0', 12);

    ++m_MetadataSet.DataPGCount;
    m_MetadataSet.DataPGIsOpen = true;
    m_Profiler.Stop("buffering");
}

void BPSerializer::SerializeData(core::IO &io, const bool advanceStep)
{
    m_Profiler.Start("buffering");
    SerializeDataBuffer(io);
    if (advanceStep)
    {
        ++m_MetadataSet.TimeStep;
        ++m_MetadataSet.CurrentStep;
    }
    m_Profiler.Stop("buffering");
}

void BPSerializer::SerializeDataBuffer(core::IO &io) noexcept
{
    // vars count and Length (only for PG)
    size_t position = m_MetadataSet.DataPGVarsCountPosition;
    const uint32_t variablesCount = m_MetadataSet.DataPGVarsCount;
    m_Data->Insert(position, variablesCount);

    // without record itself and vars count
    // Note: m_MetadataSet.DataPGVarsCount has been incremented by 4
    // in previous CopyToBuffer operation!
    const uint64_t variablesLength = static_cast<uint64_t>(
        position - m_MetadataSet.DataPGVarsCountPosition - 8);
    m_Data->Insert(position, variablesLength);

    // each attribute is only written to output once
    size_t attributesSizeInData = GetAttributesSizeInData(io);
    if (attributesSizeInData)
    {
        attributesSizeInData += 12; // count + length + end ID
        m_Data->Reserve(m_Data->Position() + attributesSizeInData + 4,
                        "when writing Attributes");
        PutAttributes(io);
    }
    else
    {
        m_Data->Reserve(m_Data->Position() + 12 + 4,
                        "when reserving for empty Attributes");
        m_Data->Insert('\0', 12);
    }

    // write a block identifier PGI]
    const std::string pgIndexEnd = "PGI]";
    m_Data->Insert(pgIndexEnd.c_str(), pgIndexEnd.size());

    // Finish writing pg group length INCLUDING the record itself and
    // including the closing padding but NOT the opening [PGI
    const uint64_t dataPGLength = position - m_MetadataSet.DataPGLengthPosition;
    m_Data->Insert(m_MetadataSet.DataPGLengthPosition, dataPGLength);
    m_MetadataSet.DataPGIsOpen = false;
}

void BPSerializer::PutAttributes(core::IO &io)
{
    const auto &attributesDataMap = io.GetAttributesDataMap();
    const size_t attributesCountPosition = m_Data->Position();

    // count is known ahead of time
    const uint32_t attributesCount =
        static_cast<uint32_t>(attributesDataMap.size());
    m_Data->Insert(attributesCount);

    // will go back
    const size_t attributesLengthPosition = m_Data->Position();
    m_Data->Insert('\0', 8); // skip attributes length

    // absolutePosition += position - attributesCountPosition;

    uint32_t memberID = 0;

    for (const auto &attributePair : attributesDataMap)
    {
        const std::string name = attributePair.first;
        const std::string type = attributePair.second.first;

        // each attribute is only written to output once
        // so filter out the ones already written
        auto it = m_SerializedAttributes.find(name);
        if (it != m_SerializedAttributes.end())
        {
            continue;
        }

        if (type == "unknown")
        {
        }
#define declare_type(T)                                                        \
    else if (type == helper::GetType<T>())                                     \
    {                                                                          \
        Stats<T> stats;                                                        \
        stats.Offset = m_Data->AbsolutePosition();                             \
        stats.MemberID = memberID;                                             \
        stats.Step = m_MetadataSet.TimeStep;                                   \
        stats.FileIndex = DataID();                                            \
        core::Attribute<T> &attribute = *io.InquireAttribute<T>(name);         \
        PutAttributeInData(attribute, stats);                                  \
        PutAttributeInIndex(attribute, stats);                                 \
    }
        ADIOS2_FOREACH_ATTRIBUTE_STDTYPE_1ARG(declare_type)
#undef declare_type

        ++memberID;
    }

    // complete attributes length
    const uint64_t attributesLength =
        static_cast<uint64_t>(m_Data->Position() - attributesLengthPosition);

    size_t backPosition = attributesLengthPosition;
    m_Data->Insert(backPosition, attributesLength);
}

uint32_t BPSerializer::DataID() const noexcept
{
    const uint32_t dataID =
        m_Aggregator.m_IsActive
            ? static_cast<uint32_t>(m_Aggregator.m_SubStreamIndex)
            : static_cast<uint32_t>(m_RankMPI);

    return dataID;
}

void BPSerializer::PutRecordDimensions(const Dims &shape, const Dims &start,
                                       const Dims &count,
                                       Buffer &buffer) noexcept
{
    for (size_t d = 0; d < count.size(); ++d)
    {
        const uint64_t countRecord = static_cast<uint64_t>(count[d]);
        const uint64_t shapeRecord =
            shape.empty() ? 0 : static_cast<uint64_t>(shape[d]);
        const uint64_t startRecord =
            start.empty() ? 0 : static_cast<uint64_t>(start[d]);

        // TODO check adios1 compatibility
        buffer.Insert(countRecord);
        buffer.Insert(shapeRecord);
        buffer.Insert(startRecord);
    }
}

} // end namespace format
} // end namespace adios2
