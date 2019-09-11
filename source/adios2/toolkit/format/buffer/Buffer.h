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

    /**
     * Returns the allocated memory size
     * @return memory reserved
     */
    virtual size_t Capacity() const noexcept = 0;

    /**
     * Return current buffer position from local 0 position (relative)
     * @return current position
     */
    virtual size_t Position() const noexcept = 0;

    /**
     * Return current buffer absolute position from absolute 0 position
     * (cumulative)
     * @return current absolute position
     */
    size_t AbsolutePosition() const noexcept;

    /**
     * Access data pointer in a non-const way (write access)
     * @return underlying buffer data pointer
     */
    virtual char *Data() noexcept;

    /**
     * Access data pointer in a const way (read-only access)
     * @return underlying buffer data pointer
     */
    virtual const char *Data() const noexcept;

    /**
     * Insert values at current position from Position(), updates the position
     * after insertion is done. This signature updates the AbsolutePosition()
     * @param source address input
     * @param elements (optional) number of elements of type T to be inserted
     * from source, default = 1
     * @param safe (optional) if true would check memory and reallocate if
     * possible, potentially slow.
     */
    template <class T>
    void Insert(const T *source, const size_t elements = 1,
                const bool safe = false);

    /**
     * Insert a repeated value at current position from Position(), updates the
     * position after insertion is done. This signature updates the
     * AbsolutePosition()
     * @param source address input
     * @param elements (optional) number of elements of type T to be inserted
     * from source, default = 1
     * @param safe (optional) if true would check memory and reallocate if
     * possible, potentially slow.
     */
    template <class T>
    void Insert(const T &source, const size_t elements = 1,
                const bool safe = false);

    /**
     * Insert values at any position, updates the position after insertion is
     * done.
     * @param source address input
     * @param elements (optional) number of elements of type T to be inserted
     * from source, default = 1
     * @param safe (optional) if true would check memory and reallocate if
     * possible, potentially slow.
     */
    template <class T>
    void Insert(size_t &position, const T *source, const size_t elements = 1,
                const bool safe = false);

    /**
     * Insert repeated values at any position, updates the position after
     * insertion is done.
     * @param source address input
     * @param elements (optional) number of elements of type T to be inserted
     * from source, default = 1
     * @param safe (optional) if true would check memory and reallocate if
     * possible, potentially slow.
     */
    template <class T>
    void Insert(size_t &position, const T &source, const size_t elements = 1,
                const bool safe = false);

    /**
     * Reads a single value for a certain position in a buffer
     * @param position
     * @param isLittleEndian
     * @return value of type T
     */
    template <class T>
    T Read(size_t &position, const bool isLittleEndian,
           const bool safe = false) const;

    /**
     * Reads a contiguous array from a certain position in a buffer
     * @param position
     * @param isLittleEndian
     * @return vector of array values of type T
     */
    template <class T>
    std::vector<T> Read(size_t &position, const size_t elements,
                        const bool isLittleEndian,
                        const bool safe = false) const;

    /**
     * Similar to C++ reserve, only reserve memory to the OS
     * @param size new size to be reserved
     * @param hint add more info to thrown exception
     */
    virtual void Reserve(const size_t size, const std::string hint);

    /**
     * Similar to C++ resize, but using reserve first to avoid power of 2 growth
     * in the capacity. New memory is initialized to 0 after this call.
     * @param size new size to be reserved
     * @param hint add more info to thrown exception
     */
    virtual void Resize(const size_t size, const std::string hint);

    /**
     * Reset the buffer Position() to zero
     * @param resetAbsolutePosition true: reset AbsolutePosition() to 0
     */
    virtual void Reset(const bool resetAbsolutePosition);

    /** Check available size for current allocation */
    virtual size_t AvailableSize() const noexcept = 0;

    /** Lock current buffer access */
    virtual void Lock();

    /** Unlock current buffer access */
    virtual void Unlock();

    /**
     * Check current buffer access
     * @return true: is locked, false: is not locked
     */
    bool IsLocked() const noexcept;

protected:
    const std::string m_Type;

    const bool m_DebugMode = true;

    size_t m_AbsolutePosition = 0;

    /** if 0: buffer can be extended, if >0: buffer has a fixed size */
    const size_t m_FixedSize = 0;

    bool m_IsLocked = false;

#define declare_map(T)                                                         \
    virtual void DoInsert(size_t &position, const T *source,                   \
                          const size_t elements, const bool safe) = 0;         \
    virtual void DoInsert(const T *source, const size_t elements,              \
                          const bool safe) = 0;

    ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_map)
#undef declare_map

#define declare_map(T, NAME)                                                   \
    virtual T DoRead##NAME(size_t &position, const bool isLittleEndian,        \
                           const bool safe) const = 0;                         \
                                                                               \
    virtual std::vector<T> DoRead##NAME(                                       \
        size_t &position, const size_t elements, const bool isLittleEndian,    \
        const bool safe) const = 0;

    ADIOS2_FOREACH_PRIMITVE_STDTYPE_2ARGS(declare_map)
#undef declare_map
};

#define declare_map(T)                                                         \
    extern template void Buffer::Insert(const T *, const size_t, const bool);  \
                                                                               \
    extern template void Buffer::Insert(const T &, const size_t, const bool);  \
                                                                               \
    extern template void Buffer::Insert(size_t &, const T *, const size_t,     \
                                        const bool);                           \
                                                                               \
    extern template void Buffer::Insert(size_t &, const T &, const size_t,     \
                                        const bool);                           \
                                                                               \
    extern template T Buffer::Read(size_t &, const bool, const bool) const;    \
                                                                               \
    extern template std::vector<T> Buffer::Read(size_t &, const size_t,        \
                                                const bool, const bool) const;

ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_map)
#undef declare_map

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BUFFER_BUFFER_H_ */
