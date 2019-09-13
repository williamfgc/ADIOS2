/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BPSerializer.tcc
 *
 *  Created on: Sep 13, 2019
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_FORMAT_BP_BPSERIALIZER_TCC_
#define ADIOS2_TOOLKIT_FORMAT_BP_BPSERIALIZER_TCC_

#include "BPSerializer.h"

namespace adios2
{
namespace format
{

// PRIVATE
template <>
inline void
BPSerializer::PutAttributeInData(const core::Attribute<std::string> &attribute,
                                 Stats<std::string> &stats) noexcept
{
    // TODO const size_t mdBeginPosition = m_Data->Position();
    // write a block identifier [AMD
    const std::string amd = "[AMD";
    m_Data->Insert(amd.c_str(), amd.size());

    const size_t attributeLengthPosition = m_Data->Position();

    // header
    m_Data->Insert('\0', 4); // skip length
    m_Data->Insert(stats.MemberID);
    InsertBPString(attribute.m_Name, *m_Data);
    m_Data->Insert('\0', 2); // skip path
    constexpr int8_t no = 'n';
    m_Data->Insert(no); // not associated with a Variable

    // here record payload offset (1 for dataType)
    stats.PayloadOffset = m_Data->AbsolutePosition() + 1;
    InsertAttributeData(attribute, *m_Data);

    // write a block identifier AMD]
    const std::string amdend = "AMD]"; // no \0
    m_Data->Insert(amdend.c_str(), amdend.size());

    // Length
    size_t backPosition = attributeLengthPosition;
    const uint32_t length =
        static_cast<uint32_t>(m_Data->Position() - attributeLengthPosition);
    m_Data->Insert(attributeLengthPosition, length);
    // TODO remove absolutePosition += position - mdBeginPosition;
}

template <class T>
void BPSerializer::PutAttributeInData(const core::Attribute<T> &attribute,
                                      Stats<T> &stats) noexcept
{
    // const size_t mdBeginPosition = m_Data->Position();

    // write a block identifier [AMD
    const std::string amd = "[AMD";
    m_Data->Insert(amd.c_str(), amd.size());

    const size_t attributeLengthPosition = m_Data->Position();

    // header
    m_Data->Insert('\0', 4); // skip length
    m_Data->Insert(stats.MemberID);
    InsertBPString(attribute.m_Name, *m_Data);
    m_Data->Insert('\0', 2); // skip path
    constexpr int8_t no = 'n';
    m_Data->Insert(no); // not associated with a Variable

    // here record payload offset (1 for dataType)
    stats.PayloadOffset = m_Data->AbsolutePosition() + 1;
    const uint8_t dataType = TypeTraits<T>::type_enum;
    m_Data->Insert(dataType);

    const uint32_t dataSize =
        static_cast<uint32_t>(attribute.m_Elements * sizeof(T));
    m_Data->Insert(dataSize);
    InsertAttributeValues(attribute, *m_Data);

    // write a block identifier AMD]
    const std::string amdend = "AMD]"; // no \0
    m_Data->Insert(amdend.c_str(), amdend.size());

    // Length
    size_t backPosition = attributeLengthPosition;
    const uint32_t length =
        static_cast<uint32_t>(m_Data->Position() - attributeLengthPosition);
    m_Data->Insert(attributeLengthPosition, length);
    // TODO remove absolutePosition += position - mdBeginPosition;
}

template <class T>
void BPSerializer::PutAttributeInIndex(const core::Attribute<T> &attribute,
                                       const Stats<T> &stats) noexcept
{
    SerialElementIndex index(stats.MemberID);
    auto &buffer = index.Buffer;

    // index.Valid = true; // when the attribute is put, set this flag to true
    const size_t lengthPosition = buffer.Position();
    buffer.Insert('\0', 4); // skip attribute length
    buffer.Insert(stats.MemberID);
    buffer.Insert('\0', 2); // skip group name
    InsertBPString(attribute.m_Name, buffer);
    buffer.Insert('\0', 2); // skip path

    uint8_t dataType = TypeTraits<T>::type_enum; // dataType

    if (dataType == type_string && !attribute.m_IsSingleValue)
    {
        dataType = type_string_array;
    }
    buffer.Insert(dataType);

    // Characteristics Sets Count in Metadata
    index.Count = 1;
    buffer.Insert(index.Count);

    // START OF CHARACTERISTICS
    const size_t characteristicsCountPosition = buffer.Position();
    buffer.Insert('\0', 1); // skip characteristics count
    buffer.Insert('\0', 4); // skip characteristics length

    uint8_t characteristicsCounter = 0;

    // DIMENSIONS
    // TODO
    PutRecordCharacteristic(characteristic_time_index, characteristicsCounter,
                            stats.Step, buffer);

    PutRecordCharacteristic(characteristic_file_index, characteristicsCounter,
                            stats.FileIndex, buffer);

    uint8_t characteristicID = characteristic_dimensions;
    buffer.Insert(characteristicID);
    constexpr uint8_t dimensions = 1;
    buffer.Insert(dimensions);
    constexpr uint16_t dimensionsLength = 24;
    buffer.Insert(dimensionsLength);

    PutRecordDimensions({attribute.m_Elements}, {}, {}, buffer);
    ++characteristicsCounter;

    // VALUE CHARACTERISTIC
    buffer.Insert(static_cast<uint8_t>(characteristic_value));
    InsertAttributeValues(attribute, buffer);
    ++characteristicsCounter;

    PutAttributeCharacteristicValueInIndex(characteristicsCounter, attribute,
                                           buffer);

    PutRecordCharacteristic(characteristic_offset, characteristicsCounter,
                            stats.Offset, buffer);

    PutRecordCharacteristic(characteristic_payload_offset,
                            characteristicsCounter, stats.PayloadOffset,
                            buffer);
    // END OF CHARACTERISTICS

    // Back to characteristics count and length
    size_t backPosition = characteristicsCountPosition;
    buffer.Insert(backPosition, characteristicsCounter);

    // remove its own length (4) + characteristic counter (1)
    const uint32_t characteristicsLength = static_cast<uint32_t>(
        buffer.Position() - characteristicsCountPosition - 4 - 1);
    buffer.Insert(characteristicsLength);

    // Remember this attribute and its serialized piece
    const uint32_t length =
        static_cast<uint32_t>(buffer.Position() - lengthPosition - 4);
    buffer.Insert(lengthPosition, length);

    m_MetadataSet.AttributesIndices.emplace(attribute.m_Name, index);
    m_SerializedAttributes.emplace(attribute.m_Name);
}

template <>
inline void BPSerializer::InsertAttributeValues(
    const core::Attribute<std::string> &attribute, Buffer &buffer) noexcept
{
    const uint8_t dataType =
        attribute.m_IsSingleValue ? type_string : type_string_array;
    buffer.Insert(dataType);

    // data
    if (dataType == type_string)
    {
        const std::string &value = attribute.m_DataSingleValue;
        const uint32_t dataSize = static_cast<uint32_t>(value.size());
        buffer.Insert(dataSize);
        buffer.Insert(value.c_str(), value.size());
    }
    else if (dataType == type_string_array)
    {
        const uint32_t elements = static_cast<uint32_t>(attribute.m_Elements);
        buffer.Insert(elements);

        for (size_t e = 0; e < elements; ++e)
        {
            // include zero terminated
            const std::string element = attribute.m_DataArray[e] + '\0';
            const uint32_t elementSize = static_cast<uint32_t>(element.size());

            buffer.Insert(elementSize);
            buffer.Insert(element.c_str(), element.size());
        }
    }
}

template <class T>
void BPSerializer::InsertAttributeValues(const core::Attribute<T> &attribute,
                                         Buffer &buffer) noexcept
{
    if (attribute.m_IsSingleValue) // single value
    {
        m_Data->Insert(attribute.m_DataSingleValue);
    }
    else // array
    {
        const std::vector<T> &array = attribute.m_DataArray;
        m_Data->Insert(array.data(), array.size());
    }
}

template <class T>
void BPSerializer::PutRecordCharacteristic(const uint8_t id, uint8_t &counter,
                                           const T &value,
                                           Buffer &buffer) noexcept
{
    buffer.Insert(id);
    buffer.Insert(value);
    ++counter;
}

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BP_BPSERIALIZER_TCC_ */
