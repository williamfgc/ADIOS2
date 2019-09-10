/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BufferSystemV.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "BufferSystemV.h"

#include <assert.h>
#include <cstring> //std::memcpy
#include <ios>     //std::ios_base::failure

#include <sys/ipc.h>   //ftok
#include <sys/shm.h>   //shmget, shmmat
#include <sys/types.h> //key_t

namespace adios2
{
namespace format
{

BufferSystemV::BufferSystemV(const size_t fixedSize, const std::string &name,
                             const unsigned int projectID, const bool remove,
                             const bool debugMode)
: Buffer("BufferSystemV", debugMode, fixedSize), m_Remove(remove)
{
    assert(projectID > 0); // for the developer
    key_t key = ftok(name.c_str(), static_cast<int>(projectID));
    m_ShmID = shmget(key, static_cast<unsigned long int>(fixedSize),
                     IPC_CREAT | 0666);
    if (m_ShmID == -1)
    {
        throw std::ios_base::failure(
            "ERROR: could not create shared memory buffer of size " +
            std::to_string(fixedSize) + " with shmget \n");
    }

    void *data = shmat(m_ShmID, nullptr, 0);
    int *status = reinterpret_cast<int *>(data);
    if (*status == -1)
    {
        throw std::runtime_error("ERROR: could not attach shared memory buffer "
                                 "to address with shmat\n");
    }
    m_Data = static_cast<char *>(data);
}

BufferSystemV::~BufferSystemV()
{
    shmdt(m_Data);

    if (m_Remove)
    {
        shmctl(m_ShmID, IPC_RMID, NULL);
    }
}

size_t BufferSystemV::Capacity() const noexcept { return m_FixedSize; }

size_t BufferSystemV::Position() const noexcept { return m_Position; }

char *BufferSystemV::Data() noexcept { return m_Data; }

const char *BufferSystemV::Data() const noexcept { return m_Data; }

void BufferSystemV::Reset(const bool resetAbsolutePosition)
{
    m_Position = 0;
    if (resetAbsolutePosition)
    {
        m_AbsolutePosition = 0;
    }
}

size_t BufferSystemV::AvailableSize() const noexcept
{
    return m_FixedSize - m_Position;
}

#define declare_map(T)                                                         \
    void BufferSystemV::DoInsert(size_t &position, const T *source,            \
                                 const size_t elements, const bool safe)       \
    {                                                                          \
        InsertCommon(position, source, elements, safe);                        \
    }                                                                          \
                                                                               \
    void BufferSystemV::DoInsert(const T *source, const size_t elements,       \
                                 const bool safe)                              \
    {                                                                          \
        InsertCommon(source, elements, safe);                                  \
    }

ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_map)
#undef declare_map

} // end namespace format
} // end namespace adios2
