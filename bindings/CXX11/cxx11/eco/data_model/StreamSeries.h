/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * StreamSeries.h
 *
 *  Created on: Apr 22, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_BINDINGS_CXX11_CXX11_ECO_DATA_MODEL_STREAMSERIES_H_
#define ADIOS2_BINDINGS_CXX11_CXX11_ECO_DATA_MODEL_STREAMSERIES_H_

#include "adios2/ADIOSConfig.h"

#include <memory>

#include "cxx11/Engine.h"
#include "cxx11/IO.h"
#include "cxx11/Variable.h"

namespace adios2
{

namespace data_model
{
class StreamSeries; // forward declare
}

namespace eco
{

/**
 * StreamSeries, class that writes and reads files with a pattern for steps in
 * the name as a stream series.
 * Example:
 * pattern:
 * file_%5d.bp
 *
 * file_00001.bp   BeginStep /EndStep
 * file_00002.bp   BeginStep /EndStep
 * ...
 * file_N.bp       BeginStep /EndStep
 */
class StreamSeries
{
public:
    /**
     * Create a StreamSeries object that interfaces adios2 full API
     * @param io to use with full API
     * @param mode
     * @param comm
     * @param pattern
     * @param patternType
     */
    StreamSeries(adios2::IO &io, const adios2::Mode mode, MPI_Comm comm,
                 const std::string &pattern,
                 const adios2::PatternType patternType);

    StreamSeries(const StreamSeries &) = delete;
    StreamSeries(const StreamSeries &&) = default;
    ~StreamSeries() = default;

    StreamSeries &operator=(const StreamSeries &) = delete;
    StreamSeries &operator=(StreamSeries &&) = default;

    std::string Pattern() const;
    std::string Name() const;
    std::string Type() const;

    StepStatus BeginStep();
    StepStatus BeginStep(const StepMode mode,
                         const float timeoutSeconds = -1.0);

    size_t CurrentStep() const;

    template <class T>
    std::vector<typename adios2::Variable<T>::Info>
    BlocksInfo(const adios2::Variable<T> variable) const;

    template <class T>
    typename adios2::Variable<T>::Span
    Put(adios2::Variable<T> variable, const size_t bufferID, const T &value);

    template <class T>
    void Put(adios2::Variable<T> variable, const T *data, const Mode launch);

    template <class T>
    void Put(const std::string &variableName, const T *data, const Mode launch);

    template <class T>
    void Put(adios2::Variable<T> variable, const T &datum, const Mode launch);

    template <class T>
    void Put(const std::string &variableName, const T &datum,
             const Mode launch);

    template <class T>
    void Get(adios2::Variable<T> variable, T *data, const Mode launch);

    template <class T>
    void Get(const std::string &variableName, T *data, const Mode launch);

    template <class T>
    void Get(adios2::Variable<T> variable, T &datum, const Mode launch);

    template <class T>
    void Get(const std::string &variableName, T &datum, const Mode launch);

    template <class T>
    void Get(adios2::Variable<T> variable, std::vector<T> &dataV,
             const Mode launch);

    template <class T>
    void Get(const std::string &variableName, std::vector<T> &dataV,
             const Mode launch);

    void PerformGets();
    void EndStep();
    void Close();

private:
    std::unique_ptr<data_model::StreamSeries> m_StreamSeries;
    std::unique_ptr<adios2::Engine> m_Engine;
};

} // end namespace eco
} // end namespace adios2

#endif /* ADIOS2_BINDINGS_CXX11_CXX11_ECO_DATA_MODEL_STREAMSERIES_H_ */
