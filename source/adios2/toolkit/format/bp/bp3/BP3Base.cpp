/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BP3Base.cpp
 *
 *  Created on: Sep 12, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "BP3Base.h"

#include "adios2/helper/adiosFunctions.h"

namespace adios2
{
namespace format
{

// PUBLIC
size_t BP3Base::GetBPIndexSizeInData(const std::string &variableName,
                                     const Dims &count) const noexcept
{
    size_t indexSize = 23; // header
    indexSize += variableName.size();

    // characteristics 3 and 4, check variable number of dimensions
    const size_t dimensions = count.size();
    indexSize += 28 * dimensions; // 28 bytes per dimension
    indexSize += 1;               // id

    // characteristics, offset + payload offset in data
    indexSize += 2 * (1 + 8);
    // characteristic 0, if scalar add value, for now only allowing string
    if (dimensions == 1)
    {
        indexSize += 2 * sizeof(uint64_t); // complex largest size
        indexSize += 1;                    // id
        indexSize += 1;                    // id
    }

    // characteristic statistics
    indexSize += 5; // count + length
    // default, only min and max and dimensions
    if (m_Parameters.StatsLevel > 0)
    {
        indexSize += 2 * (2 * sizeof(uint64_t) + 1);
        indexSize += 1 + 1; // id

        indexSize += 28 * dimensions + 1;
    }

    return indexSize + 12; // extra 12 bytes in case of attributes
}

// PRIVATE
BP3Base::ElementIndexHeader
BP3Base::ReadElementIndexHeader(const Buffer &buffer, size_t &position,
                                const bool isLittleEndian) const noexcept
{
    ElementIndexHeader header;
    header.Length = buffer.Read<uint32_t>(position, isLittleEndian);
    header.MemberID = buffer.Read<uint32_t>(position, isLittleEndian);
    header.GroupName = ReadBPString(buffer, position, isLittleEndian);
    header.Name = ReadBPString(buffer, position, isLittleEndian);
    header.Path = ReadBPString(buffer, position, isLittleEndian);
    header.DataType = buffer.Read<int8_t>(position, isLittleEndian);
    header.CharacteristicsSetsCount =
        buffer.Read<uint64_t>(position, isLittleEndian);
    return header;
}

std::string BP3Base::MetadataName(const std::string &name) const noexcept
{
    return helper::AddExtension(name, ".bp");
}

std::string BP3Base::DataName(const std::string &name, const size_t id,
                              const bool hasSubStreams) const noexcept
{
    if (!hasSubStreams)
    {
        return name;
    }

    const std::string bpName = helper::AddExtension(name, ".bp");

    // path/root.bp.dir/root.bp.Index
    std::string bpRoot = bpName;
    const auto lastPathSeparator(bpName.find_last_of(PathSeparator));

    if (lastPathSeparator != std::string::npos)
    {
        bpRoot = bpName.substr(lastPathSeparator);
    }

    const size_t index =
        m_Aggregator.m_IsActive ? m_Aggregator.m_SubStreamIndex : id;

    const std::string bpRankName(bpName + ".dir" + PathSeparator + bpRoot +
                                 "." + std::to_string(index));
    return bpRankName;
}

std::string BP3Base::BaseName(const std::string &name) const noexcept
{
    return helper::AddExtension(name, ".bp") + ".dir";
}

} // end namespace format
} // end namespace adios2
