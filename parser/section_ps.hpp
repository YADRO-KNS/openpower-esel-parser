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

#pragma once

#include "section.hpp"

namespace eSEL
{

/**
 * @class SectionPS
 * @brief Primary System Reference Code section.
 */
class SectionPS : public Section
{
  public:
    /** @brief Section Id. */
    static constexpr uint16_t SectionId = sectionId('P', 'S');

    /**
     * @struct PSRCData
     * @brief Primary System Reference Code section data.
     */
    struct PSRCData
    {
        uint8_t version;
        uint8_t flags;
        uint8_t reserved0;
        uint8_t wordCount;
        uint16_t reserved1;
        uint16_t srcLength;
        uint32_t extRefCode2;
        uint32_t extRefCode3;
        uint32_t extRefCode4;
        uint32_t extRefCode5;
        uint32_t extRefCode6;
        uint32_t extRefCode7;
        uint32_t extRefCode8;
        uint32_t extRefCode9;
        char primaryRefCode[32];
    } __attribute__((packed));

    SectionPS(const Header& header, const Payload& payload);
    std::string name() const override;

    /**
     * @brief Get unflatten section data.
     *
     * @return unflatten section data
     */
    const PSRCData& data() const;

  private:
    /** @brief Unflatten section data. */
    PSRCData data_;
};

} // namespace eSEL
