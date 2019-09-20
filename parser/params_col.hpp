/**
 * @brief Parameters collector used as callback for parsers.
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

#include <errlusrparser.H>

namespace eSEL
{

/**
 * @class ParamsCollector
 * @brief Parameters collector used as callback for parsers.
 */
class ParamsCollector : public ErrlUsrParser
{
  public:
    /**
     * @brief Constructor.
     *
     * @param[in] dst - parameters array to fill
     */
    ParamsCollector(Params& dst);

    // From ErrlUsrParser
    void PrintString(const char* name, const char* value) override;
    void PrintBool(const char* name, bool value) override;
    void PrintNumber(const char* name, const char* fmt,
                     const int value) override;
    void PrintNumberUint64(const char* name, const char* fmt,
                           uint64_t value) override;
    void PrintHexDump(const void* data, uint32_t len) override;
    void PrintHeading(const char* name) override;
    void PrintBlank() override;
    void PrintTrace(const char* trace) override;

  private:
    /**
     * @brief Set parameter with saving numeric type.
     *        Called from PrintNumber() family functions to set parameter as
     *        a number instead of storing them as string.
     *
     * @param[in] name - parameter name
     * @param[in] fmt - printf-like format
     * @param[in] value - original value
     *
     * @return true if parameter handled
     */
    bool SaveNumeric(const char* name, const char* fmt, uint64_t value);

  private:
    /** @brief Collected parameters. */
    Params& params_;
};

} // namespace eSEL
