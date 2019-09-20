/**
 * @brief Plugins interface for interaction with parser core.
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

#include <string>

#include "errlusrparser.H"

namespace eSEL
{

/**
 * @brief Parse User Defined Data section of eSEL using HostBoot's plugins.
 *
 * @param[in] cb - parameters collector callback
 * @param[in] cid - component ID
 * @param[in] sst - subsection type ID
 * @param[in] ver - section data version
 * @param[in] buffer - source data buffer to parser
 * @param[in] len - length of the buffer in bytes
 *
 * @return true if parsing completed successfully
 */
bool parseUserDefinedSection(ErrlUsrParser& cb, uint16_t cid, uint8_t sst,
                             uint8_t ver, const void* buffer, uint32_t len);

/**
 * @brief Get source description.
 *
 * @param[in] cb - parameters collector callback
 * @param[in] prRefCode - primary Reference Code
 * @param[in] extRefCode3 - extended Reference Code #3
 *
 * @return true if parsing completed successfully
 */
bool getSourceDescription(ErrlUsrParser& cb, uint32_t prRefCode,
                          const uint32_t extRefCode3);

/**
 * @brief Get component name.
 *
 * @param[in] id - component id
 *
 * @return component name, "Undefined [0xXXXX]" if not found
 */
std::string getComponentName(uint16_t id);

} // namespace eSEL
