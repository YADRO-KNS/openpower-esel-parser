/**
 * @brief User Defined Data section.
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

#include "section_ud.hpp"

#include "hexdump.hpp"
#include "params_col.hpp"

#include <hbplugins.hpp>

namespace eSEL
{

SectionUD::SectionUD(const Header& header, const Payload& payload) :
    Section(header, payload)
{
    // Parse section's payload
    ParamsCollector pc(params_);
    const bool rc = parseUserDefinedSection(pc, header_.component,
                                            header_.subtype, header_.version,
                                            payload_.data(), payload_.size());
    if (!rc)
    {
        std::string hex = hexDump(payload_.data(), payload_.size());
        params_.emplace_back(Param(Param::Raw, std::string(), std::move(hex)));
    }
}

std::string SectionUD::name() const
{
    return "User Defined Data";
}

} // namespace eSEL
