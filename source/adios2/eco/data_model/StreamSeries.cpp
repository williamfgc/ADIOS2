/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * StreamSeries.cpp
 *
 *  Created on: Apr 16, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "StreamSeries.h"

namespace adios2
{
namespace eco
{

StreamSeries::StreamSeries(core::IO &io, const std::string &pattern,
                           const adios2::Mode mode, MPI_Comm comm)
: EcoSystem("StreamSeries", &io), m_Pattern(pattern), m_Mode(mode),
  m_MPIComm(comm)
{
}

std::string StreamSeries::Pattern() const { return m_Pattern; }

std::string StreamSeries::Name() const
{
    CheckNullEngine("Name");
    return m_Engine->m_Name;
}

std::string StreamSeries::Type() const
{
    CheckNullEngine("Type");
    return m_Engine->m_EngineType;
}

StepStatus StreamSeries::BeginStep() {}

StepStatus StreamSeries::BeginStep(StepMode mode, const float timeoutSeconds) {}

size_t StreamSeries::CurrentStep() const { return m_CurrentStep; }

void StreamSeries::PerformGets()
{
    CheckNullEngine("PerformGets");
    m_Engine->PerformGets();
}

void StreamSeries::EndStep()
{
    CheckNullEngine("EndStep");
    m_Engine->Close();
    m_Engine = nullptr;
}

void StreamSeries::Close()
{
    if (m_Engine != nullptr)
    {
        if (!m_Engine->m_IsClosed)
        {
            m_Engine->Close();
        }
        m_Engine = nullptr;
    }
}

// PRIVATE
void StreamSeries::CheckNullEngine(const std::string &hint) const
{
    if (m_IO->m_DebugMode)
    {
        if (m_Engine == nullptr)
        {
            throw std::invalid_argument(
                "ERROR: in StreamSeries for pattern " + m_Pattern +
                ", current Engine is null for step " + m_CurrentStep +
                " in call to " + hint + "\n");
        }
    }
}

std::string StreamSeries::CurrentName() const {}

#define declare_template_instantiation(T)                                      \
                                                                               \
    template void StreamSeries::Put<T>(core::Variable<T> &, const T *,         \
                                       const Mode);                            \
    template void StreamSeries::Put<T>(const std::string &, const T *,         \
                                       const Mode);                            \
                                                                               \
    template void StreamSeries::Put<T>(core::Variable<T> &, const T &,         \
                                       const Mode);                            \
    template void StreamSeries::Put<T>(const std::string &, const T &,         \
                                       const Mode);                            \
                                                                               \
    template void StreamSeries::Get<T>(core::Variable<T> &, T *, const Mode);  \
    template void StreamSeries::Get<T>(const std::string &, T *, const Mode);  \
                                                                               \
    template void StreamSeries::Get<T>(core::Variable<T> &, T &, const Mode);  \
    template void StreamSeries::Get<T>(const std::string &, T &, const Mode);  \
                                                                               \
    template void StreamSeries::Get<T>(core::Variable<T> &, std::vector<T> &,  \
                                       const Mode);                            \
                                                                               \
    template void StreamSeries::Get<T>(const std::string &, std::vector<T> &,  \
                                       const Mode);                            \
                                                                               \
    template std::vector<typename core::Variable<T>::Info>                     \
    StreamSeries::BlocksInfo(const core::Variable<T> &) const;

ADIOS2_FOREACH_STDTYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

#define declare_template_instantiation(T)                                      \
    template typename core::Variable<T>::Span &StreamSeries::Put(              \
        core::Variable<T> &, const size_t, const T &);

ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

} // end namespace eco
} // end namespace adios2
