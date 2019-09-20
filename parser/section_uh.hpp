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

#pragma once

#include "section.hpp"

namespace eSEL
{

/**
 * @class SectionUH
 * @brief User Header section.
 */
class SectionUH : public Section
{
  public:
    /** @brief Section Id. */
    static constexpr uint16_t SectionId = sectionId('U', 'H');

    /** @struct UHData
     *  @brief User Header section data.
     */
    struct UHData
    {
        uint8_t subsystemId; ///< Subsystem id
        uint8_t eventData;
        uint8_t eventSeverity; ///< Event severity
        uint8_t eventType;     ///< Event type
        uint32_t reserved0;
        uint8_t problemDomain;
        uint8_t problemVector;
        uint16_t action; ///< Action code
        uint32_t reserved1;
    } __attribute__((packed));

    SectionUH(const Header& header, const Payload& payload);
    std::string name() const override;

    /** @brief Get unflatten section data.
     *
     *  @return unflatten section data
     */
    const UHData& data() const;

  private:
    /** @brief Unflatten section data */
    UHData data_;
};

} // namespace eSEL
