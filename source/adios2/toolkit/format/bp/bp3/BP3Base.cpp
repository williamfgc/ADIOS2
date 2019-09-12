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
// PRIVATE
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
