/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BPVizWriter.h
 *
 *  Created on: Jan 9, 2018
 *      Author: Dave Pugmire
 *              James Kress
 *              William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_ENGINE_BPVIZ_BPVIZWRITER_H_
#define ADIOS2_ENGINE_BPVIZ_BPVIZWRITER_H_

#include "../../toolkit/analytics/vtkm/VizVTKm.h"
#include "adios2/ADIOSConfig.h"
#include "adios2/core/Engine.h"
#include "adios2/toolkit/format/bp3/BP3.h"
#include "adios2/toolkit/transportman/TransportMan.h" //transport::TransportsMan

namespace adios2
{

class BPVizWriter : public Engine
{

public:
    /**
     * Constructor for file Writer in BP format
     * @param name unique name given to the engine
     * @param openMode w (supported), r, a from OpenMode in ADIOSTypes.h
     * @param mpiComm MPI communicator
     */
    BPVizWriter(IO &io, const std::string &name, const Mode mode,
                MPI_Comm mpiComm);

    ~BPVizWriter();

    StepStatus BeginStep(StepMode mode, const float timeoutSeconds = 0.f) final;
    void PerformPuts() final;
    void EndStep() final;

    void Close(const int transportIndex = -1) final;

private:
    /** Single object controlling BP buffering */
    format::BP3Serializer m_BP3Serializer;

    /** Manage BP data files Transports from IO AddTransport */
    transportman::TransportMan m_FileDataManager;

    /** Manages the optional collective metadata files */
    transportman::TransportMan m_FileMetadataManager;

    /** Manages Operators on Variables for visualization with VTK-m */
    viz::VizVTKm m_VisVTKm;

    void Init() final;

    /** Parses parameters from IO SetParameters */
    void InitParameters() final;
    /** Parses transports and parameters from IO AddTransport */
    void InitTransports() final;
    /** Allocates memory and starts a PG group */
    void InitBPBuffer();

#define declare_type(T)                                                        \
    void DoPutSync(Variable<T> &, const T *) final;                            \
    void DoPutDeferred(Variable<T> &, const T *) final;                        \
    void DoPutDeferred(Variable<T> &, const T &) final;
    ADIOS2_FOREACH_TYPE_1ARG(declare_type)
#undef declare_type

    /**
     * Common function for primitive PutSync, puts variables in buffer
     * @param variable
     * @param values
     */
    template <class T>
    void PutSyncCommon(Variable<T> &variable, const T *values);

    template <class T>
    void PutDeferredCommon(Variable<T> &variable, const T *values);

    /** Write a profiling.json file from m_BP1Writer and m_TransportsManager
     * profilers*/
    void WriteProfilingJSONFile();

    void WriteCollectiveMetadataFile();
};

} // end namespace adios2

#endif /* ADIOS2_ENGINE_BP_BPFILEWRITER_H_ */
