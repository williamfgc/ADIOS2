/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BufferSystemV.h
 *
 *  Created on: Jul 9, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_FORMAT_BUFFER_IPC_BUFFERSYSTEMV_H_
#define ADIOS2_TOOLKIT_FORMAT_BUFFER_IPC_BUFFERSYSTEMV_H_

#include "adios2/toolkit/format/buffer/Buffer.h"

namespace adios2
{
namespace format
{

class BufferSystemV : public Buffer
{
public:
    BufferSystemV(const size_t fixedSize, const std::string &name,
                  const unsigned int projectID, const bool remove,
                  const bool debugMode);

    ~BufferSystemV();

    size_t Capacity() const noexcept final;

    size_t Position() const noexcept final;

    char *Data() noexcept final;

    const char *Data() const noexcept final;

    void Reset(const bool resetAbsolutePosition) final;

    size_t AvailableSize() const noexcept;

private:
    /** current relative position counter */
    size_t m_Position = 0;

    /** shared memory segment ID from shmget */
    int m_ShmID = -1;

    /** pointer to shared memory segment */
    char *m_Data = nullptr;

    /** true: remove with destructor (RAII), false: make it persistent */
    const bool m_Remove;

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
};

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BUFFER_IPC_BUFFERSYSTEMV_H_ */
