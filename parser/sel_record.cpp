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

#include "sel_record.hpp"

#include "fmtexcept.hpp"

#include <cinttypes>

namespace eSEL
{

SelRecord::SelRecord(const uint8_t* data, size_t len)
{
    if (len < sizeof(SelRecord))
    {
        throw InvalidFormat("Buffer too small to fit SEL record: %" PRIu32
                            " bytes, expected %" PRIu32,
                            len, sizeof(SelRecord));
    }

    *this = *reinterpret_cast<const SelRecord*>(data);
    recordId = be16toh(recordId);
    timeStamp = be32toh(timeStamp);
    generatorId = be16toh(generatorId);
}

Params SelRecord::params() const
{
    // clang-format off
    return Params{{"Record ID", recordId},
                  {"Record type", recordType},
                  {"Timestamp", timeStamp},
                  {"Generator ID", generatorId},
                  {"Event revision", eventRevision},
                  {"Sensor type", sensorType},
                  {"Sensor number", sensorNum},
                  {"Event type", eventType},
                  {"Event data 1", eventData1},
                  {"Event data 2", eventData2},
                  {"Event data 3", eventData3}};
    // clang-format on
}

} // namespace eSEL
