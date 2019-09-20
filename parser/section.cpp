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

#include "section.hpp"

#include "ltables.hpp"

#include <hbplugins.hpp>

namespace eSEL
{

Section::Section(const Header& header, const Payload& payload) :
    header_(header), payload_(payload)
{
}

std::string Section::name() const
{
    return "General data (unknown section type)";
}

const Section::Header& Section::header() const
{
    return header_;
}

Params Section::headerParams() const
{
    std::string sectionId = toHex(header_.id);
    const char idLo = (header_.id >> 8) & 0xff;
    const char idHi = header_.id & 0xff;
    if (std::isprint(idHi) && std::isprint(idLo))
    {
        sectionId += " (";
        sectionId += idLo;
        sectionId += idHi;
        sectionId += ')';
    }

    return Params{{"Section ID", sectionId},
                  {"Section length", header_.length},
                  {"Section version", header_.version},
                  {"Section subtype", header_.subtype},
                  {"Section component", getComponentName(header_.component)}};
}

const Section::Payload& Section::payload() const
{
    return payload_;
}

Params Section::payloadParams() const
{
    return params_;
}

} // namespace eSEL
