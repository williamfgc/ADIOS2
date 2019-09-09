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
}

} // end namespace format
} // end namespace adios2
