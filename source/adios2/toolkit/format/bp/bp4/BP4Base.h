/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BP4Base.h
 *
 *  Created on: Sep 12, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_FORMAT_BP_BP4_BP4BASE_H_
#define ADIOS2_TOOLKIT_FORMAT_BP_BP4_BP4BASE_H_

#include "adios2/toolkit/format/bp/BPBase.h"

namespace adios2
{
namespace format
{

class BP4Base : public BPBase
{
public:
    BP4Base(helper::Comm const &comm, const bool debugMode);

    virtual ~BP4Base() = default;

private:
    std::string MetadataName(const std::string &name) const noexcept final;

    std::string DataName(const std::string &name, const size_t id,
                         const bool hasSubStreams) const noexcept final;

    std::string BaseName(const std::string &name) const noexcept final;
};

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BP_BP4_BP4BASE_H_ */
