/**
 * @brief Base representation of section in eSEL (Platform Event Log entry).
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

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace eSEL
{

/**
 * @class Section
 * @brief Base representation of a eSEL section.
 */
class Section
{
  public:
    /** @brief Expression to make 16-bit section Id from two 8-bit parts. */
    static constexpr uint16_t sectionId(uint8_t h, uint8_t l) noexcept
    {
        return static_cast<uint16_t>(h) << 8 | l;
    }

    /**
     * @struct Header
     * @brief Section header.
     */
    struct Header
    {
        uint16_t id;        ///< Section Id
        uint16_t length;    ///< Section length, include header
        uint8_t version;    ///< Section version
        uint8_t subtype;    ///< Section sub-type id
        uint16_t component; ///< Component id of section creator
    } __attribute__((packed));

    /** @brief Section payload data buffer */
    using Payload = std::vector<uint8_t>;

    /** @brief Constructor.
     *
     *  @param[in] header - section header
     *  @param[in] payload - section payload data
     *
     *  @throws InvalidFormat if header or payload contains wrong data
     */
    Section(const Header& header, const Payload& payload);

    /** @brief Destructor. */
    virtual ~Section() = default;

    /** @brief Get section type name.
     *
     *  @return section type name
     */
    virtual std::string name() const;

    /** @brief Get section header as a structure.
     *
     *  @return section header
     */
    const Header& header() const;

    /** @brief Get human readable description of section's header.
     *
     *  @return array of human readable parameters of section's header
     */
    Params headerParams() const;

    /** @brief Get section payload data.
     *
     *  @return section payload data buffer
     */
    const Payload& payload() const;

    /** @brief Get human readable description of section's payload.
     *
     *  @return array of human readable parameters of section's payload
     */
    Params payloadParams() const;

  protected:
    /** @brief Section's header. */
    Header header_;
    /** @brief Section's payload data. */
    Payload payload_;
    /** @brief Human readable section's payload data. */
    Params params_;
};

// Sections array
using Sections = std::vector<std::shared_ptr<Section>>;

} // namespace eSEL
