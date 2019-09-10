/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BufferSystemV.tcc
 *
 *  Created on: Sep 10, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_FORMAT_BUFFER_IPC_BUFFERSYSTEMV_TCC_
#define ADIOS2_TOOLKIT_FORMAT_BUFFER_IPC_BUFFERSYSTEMV_TCC_

#include "BufferSystemV.h"

#include <cstring> // std::memcpy

namespace adios2
{
namespace format
{

template <class T>
void BufferSystemV::InsertCommon(size_t &position, const T *source,
                                 const size_t elements, const bool safe)
{
    const size_t count = elements * sizeof(T);

    if (safe)
    {
        const size_t newCapacity = position + count;
        if (newCapacity > m_FixedSize)
        {
            throw std::invalid_argument(
                "ERROR: requested new size " + std::to_string(newCapacity) +
                " is larger than SystemV shared memory fixed size " +
                std::to_string(m_FixedSize) + "\n");
        }
    }

    std::memcpy(m_Data + position, source, count);
    position += count;
}

template <class T>
void BufferSystemV::InsertCommon(const T *source, const size_t elements,
                                 const bool safe)
{
    InsertCommon(m_Position, source, elements, safe);
}

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BUFFER_IPC_BUFFERSYSTEMV_TCC_ */
