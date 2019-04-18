/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * StreamSeries.tcc
 *
 *  Created on: Apr 16, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_ECO_DATA_MODEL_STREAMSERIES_TCC_
#define ADIOS2_ECO_DATA_MODEL_STREAMSERIES_TCC_

#include "StreamSeries.h"

namespace adios2
{
namespace eco
{

template <class T>
typename core::Variable<T>::Span &StreamSeries::Put(core::Variable<T> &variable,
                                                    const size_t bufferID,
                                                    const T &value)
{
    CheckNullEngine("Put span");
    m_Engine->Put(variable, bufferID, value);
}

template <class T>
void StreamSeries::Put(core::Variable<T> &variable, const T *data,
                       const Mode launch)
{
    CheckNullEngine("Put");
    m_Engine->Put(variable, data, launch);
}

template <class T>
void StreamSeries::Put(const std::string &variableName, const T *data,
                       const Mode launch)
{
    CheckNullEngine("Put");
    m_Engine->Put(variableName, data, launch);
}

template <class T>
void StreamSeries::Put(core::Variable<T> &variable, const T &datum,
                       const Mode launch)
{
    CheckNullEngine("Put");
    m_Engine->Put(variable, datum, launch);
}

template <class T>
void StreamSeries::Put(const std::string &variableName, const T &datum,
                       const Mode launch)
{
    CheckNullEngine("Put");
    m_Engine->Put(variableName, datum, launch);
}

template <class T>
void StreamSeries::Get(core::Variable<T> &variable, T *data, const Mode launch)
{
    CheckNullEngine("Get");
    m_Engine->Get(variable, data, launch);
}

template <class T>
void StreamSeries::Get(const std::string &variableName, T *data,
                       const Mode launch)
{
    CheckNullEngine("Get");
    m_Engine->Get(variableName, data, launch);
}

template <class T>
void StreamSeries::Get(core::Variable<T> &variable, T &datum, const Mode launch)
{
    CheckNullEngine("Get");
    m_Engine->Get(variable, datum, launch);
}

template <class T>
void StreamSeries::Get(const std::string &variableName, T &datum,
                       const Mode launch)
{
    CheckNullEngine("Get");
    m_Engine->Get(variableName, datum, launch);
}

template <class T>
void StreamSeries::Get(core::Variable<T> &variable, std::vector<T> &dataV,
                       const Mode launch)
{
    CheckNullEngine("Get");
    m_Engine->Get(variable, dataV, launch);
}

template <class T>
void StreamSeries::Get(const std::string &variableName, std::vector<T> &dataV,
                       const Mode launch)
{
    CheckNullEngine("Get");
    m_Engine->Get(variableName, dataV, launch);
}

} // namespace eco
} // namespace adios2

#endif /* ADIOS2_ECO_DATA_MODEL_STREAMSERIES_TCC_ */
