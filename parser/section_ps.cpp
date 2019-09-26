/**
 * @brief Primary System Reference Code section.
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

#include "section_ps.hpp"

#include "fmtexcept.hpp"
#include "ltables.hpp"
#include "params_col.hpp"

#include <cinttypes>
#include <hbplugins.hpp>

namespace eSEL
{

SectionPS::SectionPS(const Header& header, const Payload& payload) :
    Section(header, payload)
{
    if (payload_.size() != sizeof(PSRCData))
    {
        throw InvalidFormat("Incompatible section payload size: %" PRIu32
                            " bytes, expected %" PRIu32,
                            payload_.size(), sizeof(PSRCData));
    }

    // Convert big endian to host format
    data_ = *reinterpret_cast<const PSRCData*>(payload_.data());
    data_.srcLength = be16toh(data_.srcLength);
    data_.extRefCode2 = be32toh(data_.extRefCode2);
    data_.extRefCode3 = be32toh(data_.extRefCode3);
    data_.extRefCode4 = be32toh(data_.extRefCode4);
    data_.extRefCode5 = be32toh(data_.extRefCode5);
    data_.extRefCode6 = be32toh(data_.extRefCode6);
    data_.extRefCode7 = be32toh(data_.extRefCode7);
    data_.extRefCode8 = be32toh(data_.extRefCode8);
    data_.extRefCode9 = be32toh(data_.extRefCode9);

    // Construct human readable descriptions
    std::string rcText(data_.primaryRefCode,
                       data_.primaryRefCode + sizeof(data_.primaryRefCode));
    uint32_t rcNum = 0;
    try
    {
        rcNum = std::stoul(rcText, 0, 16);
        params_.emplace_back(
            Param{"Module ID",
                  getComponentName(static_cast<uint16_t>(rcNum & 0xff00))});
        params_.emplace_back(Param{"Reference code", rcNum});
    }
    catch (const std::exception&)
    {
        params_.emplace_back(Param{"Reference code", rcText});
    }

    // clang-format off
    params_.emplace_back(Param{"Flags", data_.flags});
    params_.emplace_back(Param{"Valid word count", data_.wordCount});
    params_.emplace_back(Param{"Words 2-5",
                               toHex(data_.extRefCode2, false) + " " +
                               toHex(data_.extRefCode3, false) + " " +
                               toHex(data_.extRefCode4, false) + " " +
                               toHex(data_.extRefCode5, false)});
    params_.emplace_back(Param{"Words 6-9",
                               toHex(data_.extRefCode6, false) + " " +
                               toHex(data_.extRefCode7, false) + " " +
                               toHex(data_.extRefCode8, false) + " " +
                               toHex(data_.extRefCode9, false)});
    // clang-format on

    if (rcNum)
    {
        ParamsCollector pc(params_);
        getSourceDescription(pc, rcNum, data_.extRefCode3);
    }
}

std::string SectionPS::name() const
{
    return "Primary System Reference Code";
}

const SectionPS::PSRCData& SectionPS::data() const
{
    return data_;
}

} // namespace eSEL
