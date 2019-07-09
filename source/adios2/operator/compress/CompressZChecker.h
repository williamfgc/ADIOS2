/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * CompressZChecker.h
 *
 *  Created on: Jul 9, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_OPERATOR_COMPRESS_COMPRESSZCHECKER_H_
#define ADIOS2_OPERATOR_COMPRESS_COMPRESSZCHECKER_H_

#include <memory>

#include "adios2/core/Operator.h"

namespace adios2
{
namespace core
{
namespace compress
{

class CompressZChecker : public Operator
{
public:
    /**
     *
     * @param paramers input z-checker parameters from user
     * @param debugMode
     * @param op sibling/managed operator: sz, zfp, mgard coming from
     * core::ADIOS class
     */
    CompressZChecker(const Params &paramers, const bool debugMode,
                     Operator &op);
    ~CompressZChecker() = default;

    // add Z-Checker functionality
    size_t Compress(const void *dataIn, const Dims &dimensions,
                    const size_t elementSize, const std::string type,
                    void *bufferOut, const Params &parameters,
                    Params &info) const final;

private:
    Operator &m_Operator;
};

} // end namespace compress
} // end namespace core
} // end namespace adios2

#endif /* ADIOS2_OPERATOR_COMPRESS_COMPRESSZCHECKER_H_ */
