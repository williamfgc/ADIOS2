/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BPVizWriter.tcc implementation of template functions with known type
 *
 *  Created on: May 22, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_ENGINE_BPVIZ_BPVIZWRITER_TCC_
#define ADIOS2_ENGINE_BPVIZ_BPVIZWRITER_TCC_

#include "BPVizWriter.h"

namespace adios2
{

template <class T>
void BPVizWriter::PutSyncCommon(Variable<T> &variable, const T *values)
{
    // set variable
    variable.SetData(values);

    // if first timestep Write create a new pg index
    if (!m_BP3Serializer.m_MetadataSet.DataPGIsOpen)
    {
        m_BP3Serializer.PutProcessGroupIndex(
            m_IO.m_HostLanguage, m_FileDataManager.GetTransportsTypes());
    }

    const size_t dataSize = variable.PayloadSize() +
                            m_BP3Serializer.GetVariableBPIndexSize(
                                variable.m_Name, variable.m_Count);
    format::BP3Base::ResizeResult resizeResult = m_BP3Serializer.ResizeBuffer(
        dataSize, "in call to variable " + variable.m_Name + " PutSync");

    if (resizeResult == format::BP3Base::ResizeResult::Flush)
    {
        m_BP3Serializer.SerializeData(m_IO);
        m_FileDataManager.WriteFiles(m_BP3Serializer.m_Data.m_Buffer.data(),
                                     m_BP3Serializer.m_Data.m_Position);
        m_BP3Serializer.ResetBuffer(m_BP3Serializer.m_Data);
        // new group index for incoming variable
        m_BP3Serializer.PutProcessGroupIndex(
            m_IO.m_HostLanguage, m_FileDataManager.GetTransportsTypes());
    }

    // WRITE INDEX to data buffer and metadata structure (in memory)//
    m_BP3Serializer.PutVariableMetadata(variable);
    m_BP3Serializer.PutVariablePayload(variable);
}

template <class T>
void BPVizWriter::PutDeferredCommon(Variable<T> &variable, const T *values)
{
    variable.SetData(values);
    m_BP3Serializer.m_DeferredVariables.push_back(variable.m_Name);
    m_BP3Serializer.m_DeferredVariablesDataSize +=
        variable.PayloadSize() +
        m_BP3Serializer.GetVariableBPIndexSize(variable.m_Name,
                                               variable.m_Count);
}

} // end namespace adios2

#endif /* ADIOS2_ENGINE_BP_BPFILEWRITER_TCC_ */
