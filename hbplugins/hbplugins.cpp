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

#include "hbplugins.hpp"

#include "errlplugins.hpp"

namespace eSEL
{

bool parseUserDefinedSection(ErrlUsrParser& cb, uint16_t cid, uint8_t sst,
                             uint8_t ver, const void* buffer, uint32_t len)
{
    const DataPlugins& factory = DataPlugins::instance();
    auto fx = factory.get(cid);
    if (fx)
        return fx(cb, const_cast<void*>(buffer), len, ver, sst);
    return false;
}

bool getSourceDescription(ErrlUsrParser& cb, uint32_t prRefCode,
                          const uint32_t extRefCode3)
{
    const uint16_t cid = static_cast<uint16_t>(prRefCode & 0xff00);
    const SrciSrc src = SrciSrc(prRefCode, extRefCode3);

    const SrcPlugins& factory = SrcPlugins::instance();
    auto fx = factory.get(cid);
    if (fx)
        return fx(cb, src);
    return false;
}

} // namespace eSEL
