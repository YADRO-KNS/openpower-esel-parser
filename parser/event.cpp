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

#include "event.hpp"

#include "section_ph.hpp"
#include "section_ps.hpp"
#include "section_ud.hpp"
#include "section_uh.hpp"

#include <endian.h>

namespace eSEL
{

/** @brief Create typed section instance.
 *
 *  @return typed section pointer
 */
static std::unique_ptr<Section> createSection(const Section::Header& header,
                                              const Section::Payload& payload)
{
    Section* section;
    switch (header.id)
    {
        case SectionPH::SectionId:
            section = new SectionPH(header, payload);
            break;
        case SectionPS::SectionId:
            section = new SectionPS(header, payload);
            break;
        case SectionUH::SectionId:
            section = new SectionUH(header, payload);
            break;
        case SectionUD::SectionId:
            section = new SectionUD(header, payload);
            break;
        default:
            section = new Section(header, payload);
    }

    return std::unique_ptr<Section>(section);
}

/** @brief Construct section instance from eSEL BLOB stream.
 *
 *  @param[in] data - pointer to eSEL BLOB data
 *  @param[in] len - size of eSEL BLOB data in bytes
 *
 *  @return typed section pointer
 *
 *  @throws InvalidFormat if there is an error in eSEL BLOB
 */
static std::unique_ptr<Section> fetchSection(const uint8_t* data, size_t len)
{
    if (len <= sizeof(Section::Header))
    {
        throw InvalidFormat(
            "Input buffer (%zu bytes) is smaller than header size (%zu)", len,
            sizeof(Section::Header));
    }

    // Read section header
    Section::Header header = *reinterpret_cast<const Section::Header*>(data);
    header.id = be16toh(header.id);
    header.length = be16toh(header.length);
    header.component = be16toh(header.component);

    // Check section header
    if (header.length <= sizeof(Section::Header))
    {
        throw InvalidFormat("Section length (%u) is too small",
                            static_cast<uint16_t>(header.length));
    }
    if (header.length > len)
    {
        throw InvalidFormat(
            "Section length (%u) is bigger then buffer size (%zu)",
            static_cast<uint16_t>(header.length), len);
    }

    // Read section payload
    const Section::Payload payload(data + sizeof(header), data + header.length);

    return createSection(header, payload);
}

void Event::parse(const uint8_t* data, size_t len)
{
    // Check input parameters
    if (!data)
        throw InvalidFormat("Invalid input buffer");
    if (len < sizeof(SectionPH::PHData))
        throw InvalidFormat("eSEL buffer too small");

    size_t pos = 0;

    // eSEL always starts with Private Header, this is the first section.
    // But in some cases we may have SEL record at the top of raw data.
    uint16_t sid = *reinterpret_cast<const uint16_t*>(data);
    if (be16toh(sid) != SectionPH::SectionId)
    {
        selRecord_ = SelRecord(data, len);
        pos += sizeof(SelRecord);
    }

    // Check for starting point of the first section (Private Header)
    if (len < pos + sizeof(SectionPH::PHData))
        throw InvalidFormat("eSEL buffer too small to fit Private Header");
    sid = *reinterpret_cast<const uint16_t*>(data + pos);
    if (be16toh(sid) != SectionPH::SectionId)
        throw InvalidFormat("Private Header section not found");

    // Parse the first section (Private Header) to get sections count
    std::unique_ptr<Section> phSection = fetchSection(data + pos, len - pos);
    pos += phSection->header().length;
    const size_t sectionsCount =
        reinterpret_cast<SectionPH*>(phSection.get())->data().sectionCount;
    sections_.reserve(sectionsCount);
    sections_.emplace_back(std::move(phSection));

    // Parse remaining part of eSEL BLOB
    for (size_t i = 1 /* skip first section */; i < sectionsCount; ++i)
    {
        if (pos >= len)
            throw InvalidFormat("Unexpected buffer end at offset %zu", pos);
        std::unique_ptr<Section> section = fetchSection(data + pos, len - pos);
        pos += section->header().length;
        sections_.emplace_back(std::move(section));
    }
}

const Sections& Event::getSections() const
{
    return sections_;
}

std::optional<SelRecord> Event::getSelRecord() const
{
    return selRecord_;
}

} // namespace eSEL
