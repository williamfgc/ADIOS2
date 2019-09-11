/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BPBase.cpp
 *
 *  Created on: Sep 5, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "BPBase.h"
#include "BPBase.tcc"

#include "adios2/helper/adiosFunctions.h"

#include "adios2/toolkit/format/bp/bpOperation/compress/BPBZIP2.h"
#include "adios2/toolkit/format/bp/bpOperation/compress/BPBlosc.h"
#include "adios2/toolkit/format/bp/bpOperation/compress/BPMGARD.h"
#include "adios2/toolkit/format/bp/bpOperation/compress/BPPNG.h"
#include "adios2/toolkit/format/bp/bpOperation/compress/BPSZ.h"
#include "adios2/toolkit/format/bp/bpOperation/compress/BPZFP.h"

namespace adios2
{
namespace format
{

// PUBLIC
BPBase::SerialElementIndex::SerialElementIndex(const uint32_t memberID,
                                               const size_t bufferSize,
                                               const std::string type)
: MemberID(memberID), Buffer(BufferSTL(false))
{
    if (type == "BufferSTL")
    {
        Buffer.Reserve(bufferSize,
                       "in call to BPBase::SerialElementIndex constructor");
    }
}

BPBase::Minifooter::Minifooter(const int8_t version) : Version(version) {}

BPBase::BPBase(helper::Comm const &comm, const bool debugMode)
: m_Comm(comm), m_DebugMode(debugMode)
{
    m_RankMPI = m_Comm.Rank();
    m_SizeMPI = m_Comm.Size();
    m_Profiler.m_IsActive = true; // default
}

void BPBase::Init(const Params &parameters, const std::string hint)
{
    // Parse Parameters
    for (const auto &parameter : parameters)
    {
        const std::string key = helper::LowerCase(parameter.first);
        const std::string value = helper::LowerCase(parameter.second);

        if (key == "profile")
        {
            m_Profiler.m_IsActive = helper::StringTo<bool>(
                value, m_DebugMode, " in Parameter key=Profile " + hint);
        }
        else if (key == "profileunits")
        {
            m_Parameters.ProfileUnit =
                helper::StringToTimeUnit(value, m_DebugMode, hint);
        }
        else if (key == "opentimeoutsecs")
        {
            m_Parameters.OpenTimeoutSecs = helper::StringTo<float>(
                value, m_DebugMode,
                " in Parameter key=OpenTimeOutSecs " + hint);

            if (m_Parameters.OpenTimeoutSecs < 0.0)
            {
                m_Parameters.OpenTimeoutSecs =
                    std::numeric_limits<float>::max() / 10000;
            }
        }
        else if (key == "beginsteppollingfrequencysecs")
        {
            m_Parameters.BeginStepPollingFrequencySecs =
                helper::StringTo<float>(value, m_DebugMode,
                                        " in Parameter key=OpenTimeOutSecs " +
                                            hint);

            if (m_Parameters.BeginStepPollingFrequencySecs < 0.0)
            {
                m_Parameters.BeginStepPollingFrequencySecs = 1.0; // a second
            }
            m_Parameters.BeginStepPollingFrequencyIsSet = true;
        }
        else if (key == "buffergrowthfactor")
        {
            m_Parameters.GrowthFactor = helper::StringTo<float>(
                value, m_DebugMode,
                " in Parameter key=BufferGrowthFactor " + hint);
        }
        else if (key == "buffertype")
        {
            m_Parameters.BufferType = value;
        }
        else if (key == "initialbuffersize")
        {
            // it will resize m_Data
            m_Parameters.InitialBufferSize = helper::StringToByteUnits(
                value, m_DebugMode,
                "for Parameter key=InitialBufferSize, in call to Open");

            if (m_DebugMode && m_Parameters.InitialBufferSize <
                                   DefaultInitialBufferSize) // 16384b
            {
                throw std::invalid_argument(
                    "ERROR: wrong value for Parameter key=InitialBufferSize, "
                    "it must be larger than 16Kb (minimum default), " +
                    hint);
            }
        }
        else if (key == "maxbuffersize")
        {
            m_Parameters.MaxBufferSize = helper::StringToSizeT(
                value, m_DebugMode, " in Parameter key=MaxBufferSize " + hint);
        }
        else if (key == "threads")
        {
            m_Parameters.Threads =
                static_cast<unsigned int>(helper::StringTo<uint32_t>(
                    value, m_DebugMode, " in Parameter key=Threads " + hint));
        }
        else if (key == "asynctasks")
        {
            m_Parameters.AsyncTasks = helper::StringTo<bool>(
                value, m_DebugMode, " in Parameter key=AsyncTasks " + hint);
        }
        else if (key == "statslevel")
        {
            m_Parameters.StatsLevel =
                static_cast<unsigned int>(helper::StringTo<uint32_t>(
                    value, m_DebugMode,
                    " in Parameter key=StatsLevel " + hint));
            if (m_DebugMode &&
                (m_Parameters.StatsLevel < 0 || m_Parameters.StatsLevel > 5))
            {
                throw std::invalid_argument(
                    "ERROR: value for Parameter key=StatsLevel must be "
                    "an integer in the range [0,5], " +
                    hint);
            }
        }
        else if (key == "statsblocksize")
        {
            m_Parameters.StatsBlockSize = helper::StringToSizeT(
                value, m_DebugMode, " in Parameter key=StatsBlockSize " + hint);
        }
        else if (key == "collectivemetadata")
        {
            m_Parameters.CollectiveMetadata = helper::StringTo<bool>(
                value, m_DebugMode,
                " in Parameter key=CollectiveMetadata " + hint);
        }
        else if (key == "flushstepscount")
        {
            m_Parameters.FlushStepsCount = helper::StringToSizeT(
                value, m_DebugMode,
                " in Parameter key=FlushStepsCount " + hint);
        }
        else if (key == "substreams")
        {
            int subStreams = static_cast<int>(helper::StringTo<int32_t>(
                value, m_DebugMode, " in Parameter key=SubStreams " + hint));

            if (subStreams < 1)
            {
                subStreams = 1;
            }
            else if (subStreams > m_SizeMPI)
            {
                subStreams = m_SizeMPI;
            }
            m_Aggregator.Init(subStreams, m_Comm);
        }
        else if (key == "node-local")
        {
            m_Parameters.NodeLocal = helper::StringTo<bool>(
                value, m_DebugMode, " in Parameter key=node-local " + hint);
        }
    }

    // set timers if active
    if (m_Profiler.m_IsActive)
    {
        const TimeUnit timeUnit = m_Parameters.ProfileUnit;
        m_Profiler.m_Timers.emplace(
            "buffering", profiling::Timer("buffering", timeUnit, m_DebugMode));
        m_Profiler.m_Timers.emplace(
            "memcpy", profiling::Timer("memcpy", timeUnit, m_DebugMode));
        m_Profiler.m_Timers.emplace(
            "minmax", profiling::Timer("minmax", timeUnit, m_DebugMode));
        m_Profiler.m_Timers.emplace(
            "meta_sort_merge",
            profiling::Timer("meta_sort_merge", timeUnit, m_DebugMode));
        m_Profiler.m_Timers.emplace(
            "aggregation",
            profiling::Timer("aggregation", timeUnit, m_DebugMode));
        m_Profiler.m_Timers.emplace(
            "mkdir", profiling::Timer("mkdir", timeUnit, m_DebugMode));
        m_Profiler.m_Bytes.emplace("buffering", 0);
    }

    // set initial buffer size
    m_Profiler.Start("buffering");
    if (m_Parameters.BufferType == "bufferstl")
    {
        m_Data = new BufferSTL(false);
        m_Data->Reserve(m_Parameters.InitialBufferSize, hint);
    }
    else
    {
        if (m_DebugMode)
        {
            throw std::invalid_argument("ERROR: buffer type " +
                                        m_Parameters.BufferType +
                                        " is not supported " + hint);
        }
    }
    m_Profiler.Stop("buffering");
}

BPBase::ReserveResult BPBase::ReserveBuffer(const size_t dataIn,
                                            const std::string hint)
{
    m_Profiler.Start("buffering");
    const size_t currentSize = m_Data->Position();
    const size_t requiredSize = dataIn + currentSize;
    const size_t maxBufferSize = m_Parameters.MaxBufferSize;

    ReserveResult result = ReserveResult::Unchanged;

    if (dataIn > maxBufferSize)
    {
        throw std::runtime_error(
            "ERROR: data size: " +
            std::to_string(static_cast<float>(dataIn) / (1024. * 1024.)) +
            " Mb is too large for adios2 bp MaxBufferSize=" +
            std::to_string(static_cast<float>(maxBufferSize) /
                           (1024. * 1024.)) +
            "Mb, try increasing MaxBufferSize in call to IO SetParameters " +
            hint + "\n");
    }

    if (requiredSize <= currentSize)
    {
        // do nothing, unchanged is default
    }
    else if (requiredSize > maxBufferSize)
    {
        if (currentSize < maxBufferSize)
        {
            m_Data->Reserve(maxBufferSize, " when reserving buffer to " +
                                               std::to_string(maxBufferSize) +
                                               "bytes, " + hint + "\n");
        }
        result = ReserveResult::Flush;
    }
    else // buffer must grow
    {
        if (currentSize < maxBufferSize)
        {
            const float growthFactor = m_Parameters.GrowthFactor;
            const size_t nextSize = std::min(
                maxBufferSize, helper::NextExponentialSize(
                                   requiredSize, currentSize, growthFactor));
            m_Data->Reserve(nextSize, " when resizing buffer to " +
                                          std::to_string(nextSize) + "bytes, " +
                                          hint);
            result = ReserveResult::Success;
        }
    }

    m_Profiler.Stop("buffering");
    return result;
}

void BPBase::ResetBuffer(Buffer &buffer, const bool resetAbsolutePosition)
{
    m_Profiler.Start("buffering");
    buffer.Reset(resetAbsolutePosition);
    m_Profiler.Stop("buffering");
}

// PROTECTED
std::vector<uint8_t>
BPBase::GetTransportIDs(const std::vector<std::string> &transportsTypes) const
    noexcept
{
    auto lf_GetTransportID = [](const std::string method) -> uint8_t {
        int id = METHOD_UNKNOWN;
        if (method == "File_NULL")
        {
            id = METHOD_NULL;
        }
        else if (method == "File_POSIX")
        {
            id = METHOD_POSIX;
        }
        else if (method == "File_fstream")
        {
            id = METHOD_FSTREAM;
        }
        else if (method == "File_stdio")
        {
            id = METHOD_FILE;
        }
        else if (method == "WAN_zmq")
        {
            id = METHOD_ZMQ;
        }

        return static_cast<uint8_t>(id);
    };

    std::vector<uint8_t> transportsIDs;
    transportsIDs.reserve(transportsTypes.size());

    for (const std::string transportType : transportsTypes)
    {
        transportsIDs.push_back(lf_GetTransportID(transportType));
    }

    return transportsIDs;
}

size_t BPBase::GetProcessGroupIndexSize(const std::string name,
                                        const std::string timeStepName,
                                        const size_t transportsSize) const
    noexcept
{
    // pgIndex + list of methods (transports)
    const size_t pgSize =
        (name.length() + timeStepName.length() + 23) + (3 + transportsSize);
    return pgSize;
}

BPBase::ProcessGroupIndex
BPBase::ReadProcessGroupIndexHeader(const Buffer &buffer, size_t &position,
                                    const bool isLittleEndian) const noexcept
{
    ProcessGroupIndex index;
    index.Length = buffer.Read<uint16_t>(position, isLittleEndian);
    index.Name = ReadBPString(buffer, position, isLittleEndian);
    index.IsColumnMajor = buffer.Read<char>(position, isLittleEndian);
    index.ProcessID = buffer.Read<int32_t>(position, isLittleEndian);
    index.StepName = ReadBPString(buffer, position, isLittleEndian);
    index.Step = buffer.Read<uint32_t>(position, isLittleEndian);
    index.Offset = buffer.Read<uint64_t>(position, isLittleEndian);
    return index;
}

std::string BPBase::ReadBPString(const Buffer &buffer, size_t &position,
                                 const bool isLittleEndian) const noexcept
{
    const size_t size =
        static_cast<size_t>(buffer.Read<uint16_t>(position, isLittleEndian));

    if (size == 0)
    {
        return "";
    }

    const std::string values(&buffer.Data()[position], size);
    position += size;
    return values;
}

void BPBase::InsertBPString(const std::string &input, Buffer &buffer,
                            size_t &position)
{
    const size_t size = input.size();
    buffer.Insert(position, static_cast<uint16_t>(size));
    buffer.Insert(position, input.c_str(), size);
}

void BPBase::InsertBPString(const std::string &input, Buffer &buffer)
{
    const size_t size = input.size();
    buffer.Insert(static_cast<uint16_t>(size));
    buffer.Insert(input.c_str(), size);
}

// static members
const std::set<std::string> BPBase::m_TransformTypes = {
    {"unknown", "none", "identity", "bzip2", "sz", "zfp", "mgard", "png",
     "blosc"}};

const std::map<int, std::string> BPBase::m_TransformTypesToNames = {
    {transform_unknown, "unknown"},   {transform_none, "none"},
    {transform_identity, "identity"}, {transform_sz, "sz"},
    {transform_zfp, "zfp"},           {transform_mgard, "mgard"},
    {transform_png, "png"},           {transform_bzip2, "bzip2"},
    {transform_blosc, "blosc"}};

BPBase::TransformTypes
BPBase::TransformTypeEnum(const std::string transformType) const noexcept
{
    TransformTypes transformEnum = transform_unknown;

    for (const auto &pair : m_TransformTypesToNames)
    {
        if (pair.second == transformType)
        {
            transformEnum = static_cast<TransformTypes>(pair.first);
            break;
        }
    }
    return transformEnum;
}

std::shared_ptr<BPOperation>
BPBase::SetBPOperation(const std::string type) const noexcept
{
    std::shared_ptr<BPOperation> bpOp;
    if (type == "sz")
    {
        bpOp = std::make_shared<BPSZ>();
    }
    else if (type == "zfp")
    {
        bpOp = std::make_shared<BPZFP>();
    }
    else if (type == "mgard")
    {
        bpOp = std::make_shared<BPMGARD>();
    }
    else if (type == "bzip2")
    {
        bpOp = std::make_shared<BPBZIP2>();
    }
    else if (type == "png")
    {
        bpOp = std::make_shared<BPPNG>();
    }
    else if (type == "blosc")
    {
        bpOp = std::make_shared<BPBlosc>();
    }

    return bpOp;
}

std::map<size_t, std::shared_ptr<BPOperation>> BPBase::SetBPOperations(
    const std::vector<core::VariableBase::Operation> &operations) const
{
    std::map<size_t, std::shared_ptr<BPOperation>> bpOperations;

    for (size_t i = 0; i < operations.size(); ++i)
    {
        const std::string type = operations[i].Op->m_Type;
        std::shared_ptr<BPOperation> bpOperation = SetBPOperation(type);

        if (bpOperation) // if the result is a supported type
        {
            bpOperations.emplace(i, bpOperation);
        }
    }
    return bpOperations;
}

#define declare_template_instantiation(T)                                      \
    template BPBase::Characteristics<T>                                        \
    BPBase::ReadElementIndexCharacteristics(const Buffer &, size_t &,          \
                                            const BPBase::DataTypes,           \
                                            const bool, const bool) const;

ADIOS2_FOREACH_STDTYPE_1ARG(declare_template_instantiation)
#undef declare_template_instantiation

} // end namespace format
} // end namespace adios2
