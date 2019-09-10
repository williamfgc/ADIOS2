/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * Buffer.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "Buffer.h"

namespace adios2
{
namespace format
{

// PUBLIC
Buffer::Buffer(const std::string type, const bool debugMode,
               const size_t fixedSize)
: m_Type(type), m_DebugMode(debugMode), m_FixedSize(fixedSize)
{
}

size_t Buffer::FixedSize() const noexcept { return m_FixedSize; }

size_t Buffer::AbsolutePosition() const noexcept { return m_AbsolutePosition; }

char *Buffer::Data() noexcept { return nullptr; }

const char *Buffer::Data() const noexcept { return nullptr; }

#define declare_map(T)                                                         \
    template <>                                                                \
    void Buffer::Insert(const T *source, const size_t elements,                \
                        const bool safe)                                       \
    {                                                                          \
        DoInsert(source, elements, safe);                                      \
        m_AbsolutePosition += elements * sizeof(T);                            \
    }                                                                          \
                                                                               \
    template <>                                                                \
    void Buffer::Insert(size_t &position, const T *source,                     \
                        const size_t elements, const bool safe)                \
    {                                                                          \
        DoInsert(position, source, elements, safe);                            \
    }

ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_map)
#undef declare_map

#define declare_map(T, NAME)                                                   \
    template <>                                                                \
    T Buffer::Read(size_t &position, const bool isLittleEndian,                \
                   const bool safe) const                                      \
    {                                                                          \
        return DoRead##NAME(position, isLittleEndian, safe);                   \
    }                                                                          \
                                                                               \
    template <>                                                                \
    std::vector<T> Buffer::Read(size_t &position, const size_t elements,       \
                                const bool isLittleEndian, const bool safe)    \
        const                                                                  \
    {                                                                          \
        return DoRead##NAME(position, elements, isLittleEndian, safe);         \
    }

ADIOS2_FOREACH_PRIMITVE_STDTYPE_2ARGS(declare_map)
#undef declare_map

void Buffer::Reserve(const size_t size, const std::string hint)
{
    throw std::invalid_argument("ERROR: buffer memory of type " + m_Type +
                                " can't call Reserve " + hint + "\n");
}

void Buffer::Resize(const size_t size, const std::string hint)
{
    throw std::invalid_argument("ERROR: buffer memory of type " + m_Type +
                                " can't call Resize " + hint + "\n");
}

void Buffer::Reset(const bool resetAbsolutePosition)
{
    throw std::invalid_argument("ERROR: buffer memory of type " + m_Type +
                                " can't call Reset\n");
}

void Buffer::Lock()
{
    throw std::invalid_argument("ERROR: buffer memory of type " + m_Type +
                                " can't call Lock\n");
}

void Buffer::Unlock()
{
    throw std::invalid_argument("ERROR: buffer memory of type " + m_Type +
                                " can't call Unlock\n");
}

bool Buffer::IsLocked() const noexcept { return m_IsLocked; }

} // end namespace format
} // end namespace adios2
