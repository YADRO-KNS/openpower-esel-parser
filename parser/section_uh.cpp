/**
 * @brief User Header section.
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

#include "section_uh.hpp"

#include "fmtexcept.hpp"
#include "ltables.hpp"

#include <endian.h>

#include <cinttypes>

namespace eSEL
{

SectionUH::SectionUH(const Header& header, const Payload& payload) :
    Section(header, payload)
{
    if (payload_.size() != sizeof(UHData))
    {
        throw InvalidFormat("Incompatible section payload size: %" PRIu32
                            " bytes, expected %" PRIu32,
                            payload_.size(), sizeof(UHData));
    }
    data_ = *reinterpret_cast<const UHData*>(payload_.data());
    data_.action = be16toh(data_.action);

    // Construct human readable descriptions
    params_ = {{"Subsystem", SubsystemName.get(data_.subsystemId)},
               {"Event severity", EventSeverity.get(data_.eventSeverity)},
               {"Event type", EventType.get(data_.eventType)},
               {"Event scope", EventScope.get(data_.eventData)},
               {"Problem domain", data_.problemDomain},
               {"Problem vector", data_.problemVector},
               {"Action", data_.action}};
}

std::string SectionUH::name() const
{
    return "User Header";
}

const SectionUH::UHData& SectionUH::data() const
{
    return data_;
}

} // namespace eSEL
