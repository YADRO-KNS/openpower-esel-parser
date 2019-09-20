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

#include "hexdump.hpp"

namespace eSEL
{

std::string hexDump(const void* data, size_t len, bool offset /*= true*/,
                    bool ascii /*= true*/)
{
    std::string hexView;

    const uint8_t* buf = reinterpret_cast<const uint8_t*>(data);

    const size_t bytesInRow = 16;
    const size_t hexRowLen = bytesInRow * 2 /* hex */ +
                             bytesInRow /* spaces */ +
                             bytesInRow / 4 /* group delimiters */;

    std::string rowHex;
    rowHex.reserve(hexRowLen);
    std::string rowAscii;
    rowAscii.reserve(bytesInRow);

    for (size_t row = 0; row * bytesInRow < len; ++row)
    {
        if (!hexView.empty())
            hexView += '\n';
        const size_t rowStart = row * bytesInRow;
        const size_t rowEnd = std::min(rowStart + bytesInRow, len);
        for (size_t pos = rowStart; pos < rowEnd; ++pos)
        {
            if (pos != rowStart && pos % 4 == 0)
                rowHex += ' ';
            rowHex += toHex(buf[pos], false) + ' ';
            if (ascii)
                rowAscii += std::isprint(buf[pos]) ? buf[pos] : '.';
        }
        if (offset)
        {
            hexView += toHex(static_cast<uint16_t>(rowStart), false);
            hexView += ":   ";
        }
        rowHex.resize(hexRowLen, ' ');
        hexView += rowHex;
        if (ascii)
        {
            hexView += ' ';
            hexView += rowAscii;
        }
        rowHex.clear();
        rowAscii.clear();
    }
    return hexView;
}

} // namespace eSEL
