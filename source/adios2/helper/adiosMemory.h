/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * adiosMemory.h : Memory copy operations functions using std::copy std::insert
 * and std::memcpy
 *
 *  Created on: May 17, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_HELPER_ADIOSMEMORY_H_
#define ADIOS2_HELPER_ADIOSMEMORY_H_

/// \cond EXCLUDE_FROM_DOXYGEN
#include <string>
#include <vector>
/// \endcond

#include "adios2/ADIOSTypes.h"

namespace adios2
{
namespace helper
{

/**
 * Inserts source at the end of a buffer updating buffer.size()
 * @param buffer data destination calls insert()
 * @param source pointer to source data
 * @param elements number of elements of source type
 */
template <class T>
void InsertToBuffer(std::vector<char> &buffer, const T *source,
                    const size_t elements = 1) noexcept;

/**
 * Copies data to a specific location in the buffer updating position
 * Does not update vec.size().
 * @param buffer data destination used in std::copy
 * @param position starting position in buffer (in terms of T not bytes)
 * @param source pointer to source data
 * @param elements number of elements of source type
 */
template <class T>
void CopyToBuffer(std::vector<char> &buffer, size_t &position, const T *source,
                  const size_t elements = 1) noexcept;

/**
 * Copies data to a specific location in the buffer updating position using
 * threads.
 * Does not update vec.size().
 * @param buffer data destination used in std::copy
 * @param position starting position in buffer (in terms of T not bytes)
 * @param source pointer to source data
 * @param elements number of elements of source type
 * @param threads number of threads sharing the copy load
 */
template <class T>
void CopyToBufferThreads(std::vector<char> &buffer, size_t &position,
                         const T *source, const size_t elements = 1,
                         const unsigned int threads = 1) noexcept;

/**
 * Copy memory from a buffer at a certain input position
 * @param buffer data source
 * @param position start position to copy from buffer, modified to final
 * position
 * @param destination pointer to destination
 * @param elements  number of elements of destination type
 */
template <class T>
void CopyFromBuffer(const std::vector<char> &buffer, size_t &position,
                    T *destination, const size_t elements = 1) noexcept;

/**
 * Cast an element to uint64 and insert to a buffer
 * @param buffer data destination
 * @param element to be added to buffer
 */
template <class T>
void InsertU64(std::vector<char> &buffer, const T element) noexcept;

template <class T>
T ReadValue(const std::vector<char> &buffer, size_t &position) noexcept;

/**
 * Clips the contiguous memory corresponding to an intersection and puts it in
 * dest, where dest has a start and coun
 * @param dest to be populated (must be pre-allocated)
 * @param destStart offset selection for data pointer
 * @param destCount count selection for data pointer
 * @param contiguousMemory input contiguous memory
 * @param blockBox input block box for contiguous memory
 * @param intersectionBox between input blockBox and start,count box
 * @param isRowMajor true: contiguous data is row major, false: column major
 * @param reverseDimensions true: data and contiguousMemory have different
 * ordering column/row or row/column major, respectively.
 */
template <class T>
void ClipContiguousMemory(T *dest, const Dims &destStart, const Dims &destCount,
                          const std::vector<char> &contiguousMemory,
                          const Box<Dims> &blockBox,
                          const Box<Dims> &intersectionBox,
                          const bool isRowMajor = true,
                          const bool reverseDimensions = false);

} // end namespace helper
} // end namespace adios2

#include "adiosMemory.inl"

#endif /* ADIOS2_HELPER_ADIOSMEMORY_H_ */
