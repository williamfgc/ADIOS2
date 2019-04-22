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
: m_StreamSeries(new StreamSeries(io, mode, comm, pattern, patternType))
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

} // end namespace eco
} // end namespace adios2
