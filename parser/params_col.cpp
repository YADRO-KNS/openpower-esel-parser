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

#include "params_col.hpp"

#include "hexdump.hpp"

#include <cstring>

namespace eSEL
{

ParamsCollector::ParamsCollector(Params& dst) : params_(dst)
{
}

void ParamsCollector::PrintString(const char* name, const char* value)
{
    std::string val;

    if (value)
    {
        // Trim spaces
        while (*value && isspace(*value))
            ++value;
        size_t pos = strlen(value);
        while (pos && isspace(value[pos - 1]))
            --pos;
        val.assign(value, value + pos);
    }

    params_.emplace_back(Param{name, value});
}

void ParamsCollector::PrintBool(const char* name, bool value)
{
    params_.emplace_back(Param{name, value});
}

void ParamsCollector::PrintNumber(const char* name, const char* fmt,
                                  const int value)
{
    if (!SaveNumeric(name, fmt, static_cast<uint64_t>(value)))
    {
        const int len = snprintf(nullptr, 0, fmt, value);
        if (len > 0)
        {
            std::string txt(static_cast<size_t>(len) + 1 /* last null */, 0);
            txt.resize(snprintf(txt.data(), txt.size(), fmt, value));
            params_.emplace_back(Param{name, txt});
        }
    }
}

void ParamsCollector::PrintNumberUint64(const char* name, const char* fmt,
                                        uint64_t value)
{
    if (!SaveNumeric(name, fmt, value))
    {
        const int len = snprintf(nullptr, 0, fmt, value);
        if (len > 0)
        {
            std::string txt(static_cast<size_t>(len) + 1 /* last null */, 0);
            txt.resize(snprintf(txt.data(), txt.size(), fmt, value));
            params_.emplace_back(Param{name, txt});
        }
    }
}

void ParamsCollector::PrintHexDump(const void* data, uint32_t len)
{
    params_.emplace_back(Param(Param::Raw, std::string(), hexDump(data, len)));
}

void ParamsCollector::PrintHeading(const char* name)
{
    params_.emplace_back(Param(name));
}

void ParamsCollector::PrintBlank()
{
    params_.emplace_back(Param());
}

void ParamsCollector::PrintTrace(const char* trace)
{
    params_.emplace_back(Param(Param::Raw, std::string(), std::string(trace)));
}

bool ParamsCollector::SaveNumeric(const char* name, const char* fmt,
                                  uint64_t value)
{
    if (strchr(fmt, ' '))
        return false; // Space in format - it is a string

    // Save in a minimum allowed capacity or capacity specified in format
    if (value > std::numeric_limits<uint32_t>::max() || strstr(fmt, "16"))
        params_.emplace_back(Param{name, value});
    else if (value > std::numeric_limits<uint16_t>::max() || strchr(fmt, '8'))
        params_.emplace_back(Param{name, static_cast<uint32_t>(value)});
    else if (value > std::numeric_limits<uint8_t>::max() || strchr(fmt, '4'))
        params_.emplace_back(Param{name, static_cast<uint16_t>(value)});
    else
        params_.emplace_back(Param{name, static_cast<uint8_t>(value)});

    return true;
}

} // namespace eSEL
