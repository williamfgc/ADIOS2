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

} // end namespace format
} // end namespace adios2
