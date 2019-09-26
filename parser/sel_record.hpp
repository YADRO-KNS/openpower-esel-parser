/**
 * @brief System Event Log record (IPMI-specific).
 *
 * Copyright (c) 2019 YADRO
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "param.hpp"

namespace eSEL
{

/**
 * @class SelRecord
 * @brief System Event Log record.
 *        See IPMI specification for details.
 */
struct SelRecord
{
    /**
     * @brief Constructor: create record from raw data.
     *
     * @param[in] data - pointer to the data buffer
     * @param[in] len - size of the data buffer in bytes
     *
     * @throws InvalidFormat in case of errors
     */
    SelRecord(const uint8_t* data, size_t len);

    /**
     * @brief Get human readable description of the record.
     *
     * @return array of human readable parameters of the record
     */
    Params params() const;

    uint16_t recordId;     ///< Record Id
    uint8_t recordType;    ///< Record type
    uint32_t timeStamp;    ///< Timestamp
    uint16_t generatorId;  ///< Generator Id
    uint8_t eventRevision; ///< Event message revision
    uint8_t sensorType;    ///< Sensor type
    uint8_t sensorNum;     ///< Sensor number
    uint8_t eventType;     ///< Event type
    uint8_t eventData1;    ///< Event data 1
    uint8_t eventData2;    ///< Event data 2
    uint8_t eventData3;    ///< Event data 3
} __attribute__((packed));

} // namespace eSEL
