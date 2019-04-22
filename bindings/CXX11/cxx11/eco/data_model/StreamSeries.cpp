/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * StreamSeries.cpp
 *
 *  Created on: Apr 22, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "StreamSeries.h"
#include "StreamSeries.tcc"

#include "adios2/eco/data_model/StreamSeries.h"

namespace adios2
{
namespace eco
{

StreamSeries::StreamSeries(adios2::IO &io, const adios2::Mode mode,
                           MPI_Comm comm, const std::string &pattern,
                           const adios2::PatternType patternType)
: m_StreamSeries(new adios2::data_model::StreamSeries(*io.m_IO, mode, comm,
                                                      pattern, patternType))
{
}

std::string StreamSeries::Pattern() const { return m_StreamSeries->Pattern(); }

std::string StreamSeries::Name() const { return m_StreamSeries->Name(); }

std::string StreamSeries::Type() const { return m_StreamSeries->Type(); }

StepStatus StreamSeries::BeginStep()
{
    m_Engine.reset(new adios2::Engine(m_StreamSeries->m_Engine));
    return m_StreamSeries->BeginStep();
}

StepStatus StreamSeries::BeginStep(const StepMode mode,
                                   const float timeoutSeconds)
{
    m_Engine.reset(new adios2::Engine(m_StreamSeries->m_Engine));
    return m_StreamSeries->BeginStep(mode, timeoutSeconds);
}

size_t StreamSeries::CurrentStep() const
{
    return m_StreamSeries->CurrentStep();
}

void StreamSeries::PerformGets() { m_StreamSeries->PerformGets(); }

void StreamSeries::EndStep() { m_StreamSeries->EndStep(); }

void StreamSeries::Close() { m_StreamSeries->Close(); }

#define declare_template_instantiation(T)                                      \
                                                                               \
    template void StreamSeries::Put<T>(Variable<T>, const T *, const Mode);    \
    template void StreamSeries::Put<T>(const std::string &, const T *,         \
                                       const Mode);                            \
                                                                               \
    template void StreamSeries::Put<T>(Variable<T>, const T &, const Mode);    \
    template void StreamSeries::Put<T>(const std::string &, const T &,         \
                                       const Mode);                            \
                                                                               \
    template void StreamSeries::Get<T>(Variable<T>, T *, const Mode);          \
    template void StreamSeries::Get<T>(const std::string &, T *, const Mode);  \
                                                                               \
    template void StreamSeries::Get<T>(Variable<T>, T &, const Mode);          \
    template void StreamSeries::Get<T>(const std::string &, T &, const Mode);  \
                                                                               \
    template void StreamSeries::Get<T>(Variable<T>, std::vector<T> &,          \
                                       const Mode);                            \
                                                                               \
    template void StreamSeries::Get<T>(const std::string &, std::vector<T> &,  \
                                       const Mode);                            \
                                                                               \
    template std::vector<typename Variable<T>::Info> StreamSeries::BlocksInfo( \
        const Variable<T>) const;

ADIOS2_FOREACH_STDTYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

#define declare_template_instantiation(T)                                      \
    template typename Variable<T>::Span StreamSeries::Put(                     \
        Variable<T>, const size_t, const T &);

ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

} // end namespace eco
} // end namespace adios2
