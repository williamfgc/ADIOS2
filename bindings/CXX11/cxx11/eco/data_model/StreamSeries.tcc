/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * StreamSeries.tcc
 *
 *  Created on: Apr 22, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_BINDINGS_CXX11_CXX11_ECO_DATA_MODEL_STREAMSERIES_TCC_
#define ADIOS2_BINDINGS_CXX11_CXX11_ECO_DATA_MODEL_STREAMSERIES_TCC_

#include "StreamSeries.h"

#include "adios2/helper/adiosFunctions.h"

namespace adios2
{
namespace eco
{

template <class T>
std::vector<typename Variable<T>::Info>
StreamSeries::BlocksInfo(const Variable<T> variable) const
{
    return m_Engine->BlocksInfo(variable, 0);
}

template <class T>
typename Variable<T>::Span
StreamSeries::Put(Variable<T> variable, const size_t bufferID, const T &value)
{
    return m_Engine->Put(variable, bufferID, value);
}

template <class T>
void StreamSeries::Put(Variable<T> variable, const T *data, const Mode launch)
{
    m_Engine->Put(variable, data, launch);
}

template <class T>
void StreamSeries::Put(const std::string &variableName, const T *data,
                       const Mode launch)
{
    m_Engine->Put(variableName, data, launch);
}

template <class T>
void StreamSeries::Put(Variable<T> variable, const T &datum, const Mode launch)
{
    m_Engine->Put(variable, datum, launch);
}

template <class T>
void StreamSeries::Put(const std::string &variableName, const T &datum,
                       const Mode launch)
{
    m_Engine->Put(variableName, datum, launch);
}

template <class T>
void StreamSeries::Get(Variable<T> variable, T *data, const Mode launch)
{
    m_Engine->Put(variable, data, launch);
}

template <class T>
void StreamSeries::Get(const std::string &variableName, T *data,
                       const Mode launch)
{
    m_Engine->Get(variableName, data, launch);
}

template <class T>
void StreamSeries::Get(Variable<T> &variable, T &datum, const Mode launch)
{
    m_Engine->Get(variable, datum, launch);
}

template <class T>
void StreamSeries::Get(const std::string &variableName, T &datum,
                       const Mode launch)
{
    m_Engine->Get(variableName, datum, launch);
}

template <class T>
void StreamSeries::Get(Variable<T> variable, std::vector<T> &dataV,
                       const Mode launch)
{
    m_Engine->Get(variable, dataV, launch);
}

template <class T>
void StreamSeries::Get(const std::string &variableName, std::vector<T> &dataV,
                       const Mode launch)
{
    m_Engine->Get(variableName, dataV, launch);
}

} // end namespace eco
} // end namespace adios2

#endif /* ADIOS2_BINDINGS_CXX11_CXX11_ECO_DATA_MODEL_STREAMSERIES_TCC_ */
