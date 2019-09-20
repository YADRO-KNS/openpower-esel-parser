/**
 * @brief Hex converters.
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

namespace eSEL
{

/**
 * @brief Fast convert unsigned decimal number to hex string.
 *
 * @param[in] num - number to convert
 * @param[in] usePrefix - add prefix before value (0x)
 *
 * @return hex number string
 */
template <typename T>
static std::string toHex(T num, bool usePrefix = true)
{
    static const char prefix[] = {'0', 'x'};
    static const char hexMap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    std::string buf(
        sizeof(T) * 2 /* hex data */ + (usePrefix ? sizeof(prefix) : 0), 0);
    char* ptr = buf.data();

    if (usePrefix)
    {
        for (size_t i = 0; i < sizeof(prefix); ++i)
            *ptr++ = prefix[i];
    }

    for (size_t i = sizeof(T); i; --i)
    {
        const uint8_t byte = num >> ((i - 1) * 8 /* bits per byte */);
        *ptr++ = hexMap[byte >> 4];
        *ptr++ = hexMap[byte & 0x0f];
    }

    return buf;
}

/**
 * @brief Create hex dump.
 *
 * @param[in] data - source data buffer
 * @param[in] len - size of source buffer
 * @param[in] offset - add offset (address) info to dump
 * @param[in] ascii - add ASCII view to dump
 *
 * @return hex dump string
 */
std::string hexDump(const void* data, size_t len, bool offset = true,
                    bool ascii = true);

} // namespace eSEL
