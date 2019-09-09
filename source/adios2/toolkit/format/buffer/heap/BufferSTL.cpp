/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BufferSTL.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "BufferSTL.h"
#include "BufferSTL.tcc"

namespace adios2
{
namespace format
{

BufferSTL::BufferSTL(const bool debugMode) : Buffer("BufferSTL", debugMode) {}

char *BufferSTL::Data() noexcept { return m_Buffer.data(); }

const char *BufferSTL::Data() const noexcept { return m_Buffer.data(); }

void BufferSTL::Resize(const size_t size, const std::string hint)
{
    try
    {
        // doing this will effectively replace the STL GNU default power of 2
        // reallocation.
        m_Buffer.reserve(size);
    }
    catch (...)
    {
        // catch a bad_alloc
        std::throw_with_nested(std::runtime_error(
            "ERROR: buffer overflow when reserving to " + std::to_string(size) +
            " bytes, " + hint + "\n"));
    }
}

void BufferSTL::Resize(const size_t size, const std::string hint)
{
    try
    {
        // doing this will effectively replace the STL GNU default power of 2
        // reallocation.
        m_Buffer.reserve(size);
        // must initialize memory (secure)
        m_Buffer.resize(size, '\0');
    }
    catch (...)
    {
        // catch a bad_alloc
        std::throw_with_nested(std::runtime_error(
            "ERROR: buffer overflow when resizing to " + std::to_string(size) +
            " bytes, " + hint + "\n"));
    }
}

void BufferSTL::Reset(const bool resetAbsolutePosition,
                      const bool zeroInitialize)
{
    m_Position = 0;
    if (resetAbsolutePosition)
    {
        m_AbsolutePosition = 0;
    }
    if (zeroInitialize)
    {
        m_Buffer.assign(m_Buffer.size(), '\0');
    }
}

size_t BufferSTL::AvailableSize() const { return m_Buffer.size() - m_Position; }

#define declare_map(T)                                                         \
    void BufferSTL::DoInsert(const size_t position, const T *source,           \
                             const size_t elements)                            \
    {                                                                          \
        InsertCommon(position, source, elements);                              \
    }                                                                          \
                                                                               \
    void BufferSTL::DoInsert(const T *source, const size_t elements)           \
    {                                                                          \
        InsertCommon(source, elements);                                        \
    }

ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_map)
#undef declare_map

} // end namespace format
} // end namespace adios2
