/**
 * @brief SRC interface declaration.
 *        More info: https://github.com/open-power/errl
 *                   file errl/include/srcisrc.H
 *
 * This header file is included by HostBoot's source code.
 * All namespace/typedef/class names in this file belong to errl/hostboot
 * interfaces and cannot be changed.
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

#include <cstdint>

/**
 * @class SrciSrc
 * @brief SRC interface, currently just a holder for reference codes.
 */
class SrciSrc
{
  public:
    /**
     * @brief Constructor.
     *
     * @param[in] prRefCode - primary Reference Code
     * @param[in] extRefCode3 - extended Reference Code #3
     */
    SrciSrc(uint32_t prRefCode, uint32_t extRefCode3) :
        prRefCode_(prRefCode), extRefCode3_(extRefCode3)
    {
    }

    /**
     * @brief Get module ID.
     *
     * @return module ID, data valid only for FipS format
     */
    uint8_t moduleId() const
    {
        return (extRefCode3_ >> 8) & 0xff;
    }

    /**
     * @brief Get reason code.
     *
     * @return reason code, data valid only for FipS format
     */
    uint16_t reasonCode() const
    {
        return prRefCode_ & 0xffff;
    }

  private:
    /** @brief Primary Reference Code. */
    uint32_t prRefCode_;
    /** @brief Extended Reference Code #3. */
    uint32_t extRefCode3_;
};
