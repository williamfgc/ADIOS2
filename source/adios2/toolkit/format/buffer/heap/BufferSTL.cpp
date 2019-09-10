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

size_t BufferSTL::Capacity() const noexcept { return m_Buffer.capacity(); }

size_t BufferSTL::Position() const { return m_Buffer.size(); }

char *BufferSTL::Data() noexcept { return m_Buffer.data(); }

const char *BufferSTL::Data() const noexcept { return m_Buffer.data(); }

void BufferSTL::Reserve(const size_t size, const std::string hint)
{
    try
    {
        m_Buffer.reserve(size);
    }
    catch (...)
    {
        // catch a bad_alloc
        std::throw_with_nested(std::runtime_error(
            "ERROR: buffer failure when reserving to " + std::to_string(size) +
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
            "ERROR: buffer failure when resizing to " + std::to_string(size) +
            " bytes, " + hint + "\n"));
    }
}

void BufferSTL::Reset(const bool resetAbsolutePosition)
{
    if (resetAbsolutePosition)
    {
        m_AbsolutePosition = 0;
    }

    Resize(0, "in call to Reset to zero bytes");
}

size_t BufferSTL::AvailableSize() const noexcept
{
    return m_Buffer.capacity() - m_Buffer.size();
}

#define declare_map(T)                                                         \
    void BufferSTL::DoInsert(size_t &position, const T *source,                \
                             const size_t elements, const bool safe)           \
    {                                                                          \
        InsertCommon(position, source, elements, safe);                        \
    }                                                                          \
                                                                               \
    void BufferSTL::DoInsert(const T *source, const size_t elements,           \
                             const bool safe)                                  \
    {                                                                          \
        InsertCommon(source, elements, safe);                                  \
    }

ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_map)
#undef declare_map

#define declare_map(T, NAME)                                                   \
    T Buffer::DoRead##NAME(size_t &position, const bool isLittleEndian,        \
                           const bool safe) const                              \
    {                                                                          \
        return ReadCommon<T>(position, isLittleEndian, safe);                  \
    }                                                                          \
                                                                               \
    std::vector<T> Buffer::DoRead##NAME(                                       \
        size_t &position, const size_t elements, const bool isLittleEndian,    \
        const bool safe) const                                                 \
    {                                                                          \
        return ReadCommon<T>(position, elements, isLittleEndian, safe);        \
    }

ADIOS2_FOREACH_PRIMITVE_STDTYPE_2ARGS(declare_map)
#undef declare_map

} // end namespace format
} // end namespace adios2
