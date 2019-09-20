/**
 * @brief eSEL (OpenPOWER Platform Event Log record).
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

#include "fmtexcept.hpp"
#include "section.hpp"
#include "sel_record.hpp"

#include <optional>

namespace eSEL
{

/**
 * @class Event
 * @brief OpenPOWER Platform Event Log record, aka eSEL.
 */
class Event
{
  public:
    /**
     * @brief Parse eSEL from raw binary data.
     *
     * @param[in] data - pointer to the data buffer
     * @param[in] len - size of the data buffer in bytes
     *
     * @throws InvalidFormat in case of errors
     */
    void parse(const uint8_t* data, size_t len);

    /**
     * @brief Get sections array.
     *
     * @return array of event's sections
     */
    const Sections& getSections() const;

    /**
     * @brief Get SEL record.
     *
     * @return SEL record instance or std::nullopt if event doesn't contain one
     */
    std::optional<SelRecord> getSelRecord() const;

  private:
    /* @brief SEL record (IPMI header). */
    std::optional<SelRecord> selRecord_;
    /* @brief Array of sections. */
    Sections sections_;
};

} // namespace eSEL
