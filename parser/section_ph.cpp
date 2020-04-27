/**
 * @brief Private Header section.
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

#include "section_ph.hpp"

#include "fmtexcept.hpp"
#include "ltables.hpp"

#include <endian.h>

#include <cinttypes>

namespace eSEL
{

SectionPH::SectionPH(const Header& header, const Payload& payload) :
    Section(header, payload)
{
    if (payload_.size() != sizeof(PHData))
    {
        throw InvalidFormat("Incompatible section payload size: %" PRIu32
                            " bytes, expected %" PRIu32,
                            payload_.size(), sizeof(PHData));
    }

    // Convert big endian to host format
    data_ = *reinterpret_cast<const PHData*>(payload_.data());
    data_.createTimestamp = be64toh(data_.createTimestamp);
    data_.commitTimestamp = be64toh(data_.commitTimestamp);
    data_.creatorSubIdHi = be32toh(data_.creatorSubIdHi);
    data_.creatorSubIdLo = be32toh(data_.creatorSubIdLo);
    data_.platformId = be32toh(data_.platformId);
    data_.logEntryId = be32toh(data_.logEntryId);

    // Construct human readable descriptions
    params_ = {{"Create timestamp", data_.createTimestamp},
               {"Commit timestamp", data_.commitTimestamp},
               {"Creator subsystem", CreatorSubSys.get(data_.subsystemId)},
               {"Section count", data_.sectionCount},
               {"Creator ID Lo", data_.creatorSubIdLo},
               {"Creator ID Hi", data_.creatorSubIdHi},
               {"Platform log ID", data_.platformId},
               {"Log entry ID", data_.logEntryId}};
}

std::string SectionPH::name() const
{
    return "Private header";
}

const SectionPH::PHData& SectionPH::data() const
{
    return data_;
}

} // namespace eSEL
