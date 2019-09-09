/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * Buffer.h : abstract class for all buffer memory types
 *
 *  Created on: Jul 9, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_FORMAT_BUFFER_BUFFER_H_
#define ADIOS2_TOOLKIT_FORMAT_BUFFER_BUFFER_H_

#include "adios2/common/ADIOSConfig.h"
#include "adios2/common/ADIOSMacros.h"
#include "adios2/common/ADIOSTypes.h"

namespace adios2
{
namespace format
{

class Buffer
{
public:
    Buffer(const std::string type, const bool debugMode = true,
           const size_t fixedSize = 0);

    virtual ~Buffer() = default;

    virtual size_t FixedSize() const noexcept = 0;

    virtual size_t Position() const noexcept = 0;

    virtual size_t AbsolutePosition() const noexcept = 0;

    virtual char *Data() noexcept;
    virtual const char *Data() const noexcept;

    /**
     * Insert values at Position()
     * @param source
     * @param elements
     */
    template <class T>
    void Insert(const T *source, const size_t elements = 1);

    template <class T>
    void Insert(const size_t position, const T *source,
                const size_t elements = 1);

    virtual void Reserve(const size_t size, const std::string hint);

    virtual void Resize(const size_t size, const std::string hint);

    /**
     * Reset the buffer m_Position to zero
     * @param resetAbsolutePosition true: reset m_AbsolutePosition = 0
     * @param zeroInitialize populate current buffer contents with '\0'
     */
    virtual void Reset(const bool resetAbsolutePosition,
                       const bool zeroInitialize);

    virtual size_t AvailableSize() const;

protected:
    const std::string m_Type;

    const bool m_DebugMode = true;

    size_t m_AbsolutePosition = 0;
    size_t m_Position = 0;

    /** if 0: buffer can be extended, if >0: buffer has a fixed size */
    const size_t m_FixedSize = 0;

#define declare_map(T)                                                         \
    virtual void DoInsert(const size_t position, const T *source,              \
                          const size_t elements) = 0;                          \
    virtual void DoInsert(const T *source, const size_t elements) = 0;

    ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_map)
#undef declare_map
};

#define declare_map(T)                                                         \
    extern template void Buffer::Insert(const T *, const size_t);              \
                                                                               \
    extern template void Buffer::Insert(const size_t, const T *, const size_t);

ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_map)
#undef declare_map

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BUFFER_BUFFER_H_ */
