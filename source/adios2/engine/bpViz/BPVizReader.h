/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BPVizReader.h
 *
 *  Created on: Feb 27, 2017
 *      Author: Dave Pugmire
 *              James Kress
 *              William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_ENGINE_BPVIZ_BPVIZREADER_H_
#define ADIOS2_ENGINE_BPVIZ_BPVIZREADER_H_

#include "adios2/ADIOSConfig.h"
#include "adios2/core/Engine.h"
#include "adios2/toolkit/format/bp3/BP3.h" //format::BP1Deserializer
#include "adios2/toolkit/transportman/TransportMan.h"

namespace adios2
{

namespace utils
{
class BPLS2;
}

class BPVizReader : public Engine
{

    friend class utils::BPLS2;

public:
    /**
     * Unique constructor
     * @param io
     * @param name
     * @param openMode only read
     * @param mpiComm
     */
    BPVizReader(IO &io, const std::string &name, const Mode mode,
                MPI_Comm mpiComm);

    virtual ~BPVizReader() = default;

    StepStatus BeginStep(StepMode mode = StepMode::NextAvailable,
                         const float timeoutSeconds = 0.f) final;

    void EndStep() final;

    void PerformGets() final;

    void Close(const int transportIndex = -1);

private:
    format::BP3Deserializer m_BP3Deserializer;
    transportman::TransportMan m_FileManager;
    transportman::TransportMan m_SubFileManager;

    /** used for per-step reads, TODO: to be moved to BP3Deserializer */
    size_t m_CurrentStep = 0;
    bool m_FirstStep = true;

    void Init();
    void InitTransports();
    void InitBuffer();

#define declare_type(T)                                                        \
    void DoGetSync(Variable<T> &, T *) final;                                  \
    void DoGetDeferred(Variable<T> &, T *) final;                              \
    void DoGetDeferred(Variable<T> &, T &) final;
    ADIOS2_FOREACH_TYPE_1ARG(declare_type)
#undef declare_type

    template <class T>
    void GetSyncCommon(Variable<T> &variable, T *data);

    template <class T>
    void GetDeferredCommon(Variable<T> &variable, T *data);

    void
    ReadVariables(IO &io,
                  const std::map<std::string, SubFileInfoMap> &variablesInfo);
};

} // end namespace adios2

#endif /* ADIOS2_ENGINE_BP_BPFILEREADER_H_ */
