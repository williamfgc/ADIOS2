/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BufferSTL.h
 *
 *  Created on: Sep 26, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_FORMAT_BUFFER_HEAP_BUFFERSTL_H_
#define ADIOS2_TOOLKIT_FORMAT_BUFFER_HEAP_BUFFERSTL_H_

#include "adios2/toolkit/format/buffer/Buffer.h"

#include <vector>

namespace adios2
{
namespace format
{

class BufferSTL : public Buffer
{
public:
    BufferSTL(const bool debugMode);
    ~BufferSTL() = default;

    size_t Capacity() const noexcept final;

    size_t Position() const noexcept final;

    char *Data() noexcept final;
    const char *Data() const noexcept final;

    void Reserve(const size_t size, const std::string hint) final;
    void Resize(const size_t size, const std::string hint) final;

    void Reset(const bool resetAbsolutePosition) final;

    size_t AvailableSize() const noexcept final;

private:
    std::vector<char> m_Buffer;

#define declare_map(T)                                                         \
    void DoInsert(size_t &position, const T *source, const size_t elements,    \
                  const bool safe) final;                                      \
    void DoInsert(const T *source, const size_t elements, const bool safe)     \
        final;

    ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_map)
#undef declare_map

    template <class T>
    void InsertCommon(size_t &position, const T *source, const size_t elements,
                      const bool safe);

    template <class T>
    void InsertCommon(const T *source, const size_t elements, const bool safe);

#define declare_map(T, NAME)                                                   \
    T DoRead##NAME(size_t &position, const bool isLittleEndian,                \
                   const bool safe) const final;                               \
                                                                               \
    std::vector<T> DoRead##NAME(size_t &position, const size_t elements,       \
                                const bool isLittleEndian, const bool safe)    \
        const final;

    ADIOS2_FOREACH_PRIMITVE_STDTYPE_2ARGS(declare_map)
#undef declare_map

    template <class T>
    T ReadCommon(size_t &position, const bool isLittleEndian, const bool safe);

    template <class T>
    std::vector<T> ReadCommon(size_t &position, const size_t elements,
                              const bool isLittleEndian, const bool safe);
};

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BUFFER_HEAP_BUFFERSTL_H_ */
