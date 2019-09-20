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

#pragma once

#include "section.hpp"

namespace eSEL
{

/**
 * @class SectionPH
 * @brief Private Header section.
 */
class SectionPH : public Section
{
  public:
    /** @brief Section Id. */
    static constexpr uint16_t SectionId = sectionId('P', 'H');

    /**
     * @struct PHData
     * @brief Private Header section data.
     */
    struct PHData
    {
        uint64_t createTimestamp; ///< Creation timestamp (TB register format)
        uint64_t commitTimestamp; ///< Commit timestamp (TB register format)
        uint8_t subsystemId;      ///< Subsystem component id
        uint16_t reserved0;
        uint8_t sectionCount; ///< Number of sections in log
        uint32_t reserved1;
        uint32_t creatorSubIdHi;
        uint32_t creatorSubIdLo;
        uint32_t platformId; ///< Platform log id
        uint32_t logEntryId; ///< Unique log entry id
    } __attribute__((packed));

    SectionPH(const Header& header, const Payload& payload);
    std::string name() const override;

    /**
     * @brief Get unflatten section data.
     *
     * @return unflatten section data
     */
    const PHData& data() const;

  private:
    /** @brief Unflatten section data. */
    PHData data_;
};

} // namespace eSEL
