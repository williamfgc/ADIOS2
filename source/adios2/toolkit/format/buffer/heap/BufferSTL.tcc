/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BufferSTL.tcc
 *
 *  Created on: Sep 9, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_FORMAT_BUFFER_HEAP_BUFFERSTL_TCC_
#define ADIOS2_TOOLKIT_FORMAT_BUFFER_HEAP_BUFFERSTL_TCC_

#include "BufferSTL.h"

#include "adios2/helper/adiosFunctions.h"

namespace adios2
{
namespace format
{

template <class T>
void BufferSTL::InsertCommon(size_t &position, const T *source,
                             const size_t elements, const bool safe)
{
    if (safe)
    {
        const size_t newCapacity = position + elements * sizeof(T);
        Reserve(newCapacity, " when calling BufferSTL::Insert at position " +
                                 std::to_string(position) + "\n");
    }

    helper::InsertToBuffer(m_Buffer, position, source, elements);
}

template <class T>
void BufferSTL::InsertCommon(const T *source, const size_t elements,
                             const bool safe)
{
    if (safe)
    {
        const size_t newCapacity = m_Buffer.size() + elements * sizeof(T);
        Reserve(newCapacity, " when calling BufferSTL::Insert \n");
    }

    helper::InsertToBuffer(m_Buffer, source, elements);
}

template <class T>
T BufferSTL::ReadCommon(size_t &position, const bool isLittleEndian,
                        const bool safe)
{
    if (safe)
    {
        const size_t finalPosition = position + sizeof(T);
        if (finalPosition > m_Buffer.size())
        {
            throw std::invalid_argument(
                "ERROR: buffer overflow accessing position " +
                std::to_string(finalPosition) + " for buffer size " +
                std::to_string(m_Buffer.size()) + "\n");
        }
    }

    return helper::ReadValue<T>(m_Buffer, position, isLittleEndian);
}

template <class T>
std::vector<T> BufferSTL::ReadCommon(size_t &position, const size_t elements,
                                     const bool isLittleEndian, const bool safe)
{
}

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BUFFER_HEAP_BUFFERSTL_TCC_ */
