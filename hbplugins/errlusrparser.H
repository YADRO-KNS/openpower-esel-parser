/**
 * @brief Parameters collector interface declaration.
 *        More info: https://github.com/open-power/errl
 *                   file errl/include/errlusrparser.H
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
 * @class ErrlUsrParser
 * @brief User parser callback interface.
 */
class ErrlUsrParser
{
  public:
    ErrlUsrParser() = default;
    virtual ~ErrlUsrParser() = default;

    virtual void PrintString(const char* name, const char* value) = 0;
    virtual void PrintBool(const char* name, bool value) = 0;
    virtual void PrintNumber(const char* name, const char* fmt, int value) = 0;
    virtual void PrintNumberUint64(const char* name, const char* fmt,
                                   uint64_t value) = 0;
    virtual void PrintHexDump(const void* data, uint32_t len) = 0;
    virtual void PrintHeading(const char* name) = 0;
    virtual void PrintBlank() = 0;
    virtual void PrintTrace(const char* trace) = 0;
};
