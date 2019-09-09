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

namespace adios2
{
namespace format
{

class BufferSTL : public Buffer
{
public:
    std::vector<char> m_Buffer;

    BufferSTL(const bool debugMode);
    ~BufferSTL() = default;

    char *Data() noexcept final;
    const char *Data() const noexcept final;

    void Reserve(const size_t size, const std::string hint) final;
    void Resize(const size_t size, const std::string hint) final;

    void Reset(const bool resetAbsolutePosition,
               const bool zeroInitialize) final;

    size_t AvailableSize() const final;

private:
#define declare_map(T)                                                         \
    void DoInsert(const size_t position, const T *source,                      \
                  const size_t elements) final;                                \
    void DoInsert(const T *source, const size_t elements) final;

    ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_map)
#undef declare_map

    template <class T>
    void InsertCommon(const size_t position, const T *source,
                      const size_t elements);

    template <class T>
    void InsertCommon(const T *source, const size_t elements);
};

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BUFFER_HEAP_BUFFERSTL_H_ */
