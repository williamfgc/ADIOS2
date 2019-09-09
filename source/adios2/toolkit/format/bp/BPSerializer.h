/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BPSerializer.h
 *
 *  Created on: Sep 9, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_FORMAT_BP_BPSERIALIZER_H_
#define ADIOS2_TOOLKIT_FORMAT_BP_BPSERIALIZER_H_

#include "adios2/toolkit/format/bp/BPBase.h"

namespace adios2
{
namespace format
{

class BPSerializer : public BPBase
{
public:
    BPSerializer(helper::Comm const &comm, const bool debugMode = false);

    virtual ~BPSerializer() = default;

    /**
     *  Writes a 64 byte header into the data/metadata buffer.
     *  Must be called only when the buffer is empty.
     *  @param buffer the data or metadata buffer
     *  @param fileType a small string up to 8 characters that is
     *  concatenated to the version string
     */
    virtual void MakeHeader(Buffer &b, const std::string fileType,
                            const bool isActive);

    /**
     * Writes a process group index PGIndex and list of methods (from
     * transports). Done at Open or EndStep.
     * @param ioName from IO class, identify Process Group with IO name
     * @param hostLanguage from ADIOS class passed to IO
     * @param transportsTypes passed to get list of transport "bp methods"
     */
    void PutProcessGroupIndex(
        const std::string &ioName, const std::string hostLanguage,
        const std::vector<std::string> &transportsTypes) noexcept;
};

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BP_BPSERIALIZER_H_ */
