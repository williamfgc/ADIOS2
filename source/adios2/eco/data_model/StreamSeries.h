/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * StreamSeries.h : write and read files with a pattern for steps in the name
 * as a stream series.
 * Example:
 *      file_*.bp       Open
 * 		file_00001.bp   BeginStep /EndStep
 * 		file_00002.bp   BeginStep /EndStep
 * 		...
 * 		file_N.bp   BeginStep /EndStep
 *
 *  Created on: Apr 16, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_ECO_DATA_MODEL_STREAMSERIES_H_
#define ADIOS2_ECO_DATA_MODEL_STREAMSERIES_H_

#include "adios2/core/Engine.h"
#include "adios2/core/IO.h"
#include "adios2/core/Variable.h"
#include "adios2/eco/EcoSystem.h"
#include "adios2/helper/adiosFunctions.h"

namespace adios2
{
namespace data_model
{

class StreamSeries : public EcoSystem
{
public:
    StreamSeries(core::IO &io, const adios2::Mode mode, MPI_Comm comm,
                 const std::string &pattern, const PatternType patternType);

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

    template <class T>
    std::vector<typename core::Variable<T>::Info>
    BlocksInfo(const core::Variable<T> &variable) const;

    size_t CurrentStep() const;

    template <class T>
    typename core::Variable<T>::Span &
    Put(core::Variable<T> &variable, const size_t bufferID, const T &value);

    template <class T>
    void Put(core::Variable<T> &variable, const T *data, const Mode launch);

    template <class T>
    void Put(const std::string &variableName, const T *data, const Mode launch);

    template <class T>
    void Put(core::Variable<T> &variable, const T &datum, const Mode launch);

    template <class T>
    void Put(const std::string &variableName, const T &datum,
             const Mode launch);

    template <class T>
    void Get(core::Variable<T> &variable, T *data, const Mode launch);

    template <class T>
    void Get(const std::string &variableName, T *data, const Mode launch);

    template <class T>
    void Get(core::Variable<T> &variable, T &datum, const Mode launch);

    template <class T>
    void Get(const std::string &variableName, T &datum, const Mode launch);

    template <class T>
    void Get(core::Variable<T> &variable, std::vector<T> &dataV,
             const Mode launch);

    template <class T>
    void Get(const std::string &variableName, std::vector<T> &dataV,
             const Mode launch);

    void PerformGets();
    void EndStep();
    void Close();

private:
    adios2::Mode m_Mode;
    MPI_Comm m_MPIComm;
    const std::string m_Pattern;
    const PatternType m_PatternType;

    size_t m_StrideOut = 1;

    core::Engine *m_Engine = nullptr;
    size_t m_CurrentStep = 0;

    void CheckNullEngine(const std::string &hint) const;

    std::string CurrentName() const;
};

#define declare_template_instantiation(T)                                      \
                                                                               \
    extern template void StreamSeries::Put<T>(core::Variable<T> &, const T *,  \
                                              const Mode);                     \
    extern template void StreamSeries::Put<T>(const std::string &, const T *,  \
                                              const Mode);                     \
                                                                               \
    extern template void StreamSeries::Put<T>(core::Variable<T> &, const T &,  \
                                              const Mode);                     \
    extern template void StreamSeries::Put<T>(const std::string &, const T &,  \
                                              const Mode);                     \
                                                                               \
    extern template void StreamSeries::Get<T>(core::Variable<T> &, T *,        \
                                              const Mode);                     \
    extern template void StreamSeries::Get<T>(const std::string &, T *,        \
                                              const Mode);                     \
                                                                               \
    extern template void StreamSeries::Get<T>(core::Variable<T> &, T &,        \
                                              const Mode);                     \
    extern template void StreamSeries::Get<T>(const std::string &, T &,        \
                                              const Mode);                     \
                                                                               \
    extern template void StreamSeries::Get<T>(core::Variable<T> &,             \
                                              std::vector<T> &, const Mode);   \
                                                                               \
    extern template void StreamSeries::Get<T>(const std::string &,             \
                                              std::vector<T> &, const Mode);   \
                                                                               \
    extern template std::vector<typename core::Variable<T>::Info>              \
    StreamSeries::BlocksInfo(const core::Variable<T> &) const;

ADIOS2_FOREACH_STDTYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

#define declare_template_instantiation(T)                                      \
    extern template typename core::Variable<T>::Span &StreamSeries::Put(       \
        core::Variable<T> &, const size_t, const T &);

ADIOS2_FOREACH_PRIMITIVE_STDTYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

} // end namespace eco
} // end namespace adios2

#endif /* ADIOS2_ECO_DATA_MODEL_STREAMSERIES_H_ */
