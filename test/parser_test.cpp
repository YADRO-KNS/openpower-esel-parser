/**
 * @brief eSEL parser tests.
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

#include <event.hpp>
#include <section_ph.hpp>
#include <section_ps.hpp>
#include <section_ud.hpp>
#include <section_uh.hpp>

#include <gtest/gtest.h>

// clang-format off

////////////////////////////////////////////////////////////////////////////////
// Private Header
static const eSEL::Params phHeader{
    {"Section ID",        "0x5048 (PH)"},
    {"Section length",    "0x0030"},
    {"Section version",   "0x01"},
    {"Section subtype",   "0x00"},
    {"Section component", "fsi"}
};
static const eSEL::Params phPayload{
    {"Create timestamp",  "0x0000000a4d71e974"},
    {"Commit timestamp",  "0x0000000a4f680d96"},
    {"Subsystem ID",      "Unknown (0x42)"},
    {"Section count",     "0x10"},
    {"Creator ID Lo",     "0x00000000"},
    {"Creator ID Hi",     "0x00000000"},
    {"Platform log ID",   "0x90000047"},
    {"Log entry ID",      "0x90000047"}
};
static const std::vector<uint8_t> phData{
    /* Header */
    0x50, 0x48, 0x00, 0x30, 0x01, 0x00, 0x0a, 0x00,
    /* Payload */
    0x00, 0x00, 0x00, 0x0a, 0x4d, 0x71, 0xe9, 0x74,
    0x00, 0x00, 0x00, 0x0a, 0x4f, 0x68, 0x0d, 0x96,
    0x42, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x90, 0x00, 0x00, 0x47, 0x90, 0x00, 0x00, 0x47
};

////////////////////////////////////////////////////////////////////////////////
// User Header
static const eSEL::Params uhHeader{
    {"Section ID",        "0x5548 (UH)"},
    {"Section length",    "0x0018"},
    {"Section version",   "0x01"},
    {"Section subtype",   "0x00"},
    {"Section component", "hwpf"}
};
static const eSEL::Params uhPayload{
    {"Subsystem ID",   "Memory subsystem"},
    {"Event severity", "Unrecoverable Error"},
    {"Event type",     "Not applicable"},
    {"Event scope",    "Single platform"},
    {"Problem domain", "0xff"},
    {"Problem vector", "0xff"},
    {"Action",         "0x0000"}
};
static const std::vector<uint8_t> uhData{
    /* Header */
    0x55, 0x48, 0x00, 0x18, 0x01, 0x00, 0x09, 0x00,
    /* Payload */
    0x20, 0x03, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

////////////////////////////////////////////////////////////////////////////////
// Primary System Reference Code
static const eSEL::Params psHeader{
    {"Section ID",        "0x5053 (PS)"},
    {"Section length",    "0x0050"},
    {"Section version",   "0x01"},
    {"Section subtype",   "0x01"},
    {"Section component", "NONE"}
};
static const eSEL::Params psPayload{
    {"Module ID",        "xscom"},
    {"Reference code",   "0xbc810406"},
    {"Flags",            "0x00"},
    {"Valid word count", "0x09"},
    {"Words 2-5",        "000000e0 00000800 00000000 00200000"},
    {"Words 6-9",        "ffffffe2 80000080 00000000 0201165a"},
    {"Description",      "XSCOM access error"},
    {"Module ID",        "XSCOM_RT_DO_OP"},
    {"Reason code",      "XSCOM_RUNTIME_ERR"},
    {"Userdata1[00:31]", "Hypervisor return code"},
    {"Userdata1[32:63]", "Runtime Target"},
    {"Userdata2",        "SCOM address"}
};
static const std::vector<uint8_t> psData{
    /* Header */
    0x50, 0x53, 0x00, 0x50, 0x01, 0x01, 0x00, 0x00,
    /* Payload */
    0x02, 0x00, 0x00, 0x09, 0x04, 0x06, 0x00, 0x48,
    0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xe2, 0x80, 0x00, 0x00, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x16, 0x5a,
    0x42, 0x43, 0x38, 0x31, 0x30, 0x34, 0x30, 0x36,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
};

////////////////////////////////////////////////////////////////////////////////
//  User Defined Data (errl/strings)
static const eSEL::Params udStrHeader{
    {"Section ID",        "0x5544 (UD)"},
    {"Section length",    "0x0040"},
    {"Section version",   "0x01"},
    {"Section subtype",   "0x01"},
    {"Section component", "errl"}
};
static const eSEL::Params udStrPayload{
    {"String data", "host_discover_targets"},
    {"String data", "libistepdisp.so"},
    {"String data", "libextinitsvc.so"}
};
static const std::vector<uint8_t> udStrData{
    /* Header */
    0x55, 0x44, 0x00, 0x40, 0x01, 0x01, 0x01, 0x00,
    /* Payload */
    0x68, 0x6f, 0x73, 0x74, 0x5f, 0x64, 0x69, 0x73,
    0x63, 0x6f, 0x76, 0x65, 0x72, 0x5f, 0x74, 0x61,
    0x72, 0x67, 0x65, 0x74, 0x73, 0x00, 0x6c, 0x69,
    0x62, 0x69, 0x73, 0x74, 0x65, 0x70, 0x64, 0x69,
    0x73, 0x70, 0x2e, 0x73, 0x6f, 0x00, 0x6c, 0x69,
    0x62, 0x65, 0x78, 0x74, 0x69, 0x6e, 0x69, 0x74,
    0x73, 0x76, 0x63, 0x2e, 0x73, 0x6f, 0x00, 0x00
};

////////////////////////////////////////////////////////////////////////////////
//  User Defined Data (errl/targets)
static const eSEL::Params udTrgHeader{
    {"Section ID",        "0x5544 (UD)"},
    {"Section length",    "0x0054"},
    {"Section version",   "0x01"},
    {"Section subtype",   "0x02"},
    {"Section component", "errl"}
};
static const eSEL::Params udTrgPayload{
    {"Targdt: DIMM",         "HUID = 0x00030002"},
    {"  ATTR_CLASS",         "CLASS_LOGICAL_CARD"},
    {"  ATTR_TYPE",          "TYPE_DIMM"},
    {"  ATTR_MODEL",         "MODEL_JEDEC"},
    {"  ATTR_PHYS_PATH",     "Physical:/Sys0/Node0/DIMM2"},
    {"  ATTR_AFFINITY_PATH", "Logical:/Sys0/Node0/Proc0/MCS0/Membuf0/MBA0/DIMM2"}
};
static const std::vector<uint8_t> udTrgData{
    /* Header */
    0x55, 0x44, 0x00, 0x54, 0x01, 0x02, 0x01, 0x00,
    /* Payload */
    0xee, 0xee, 0xee, 0xee, 0x54, 0x61, 0x72, 0x67,
    0x64, 0x74, 0x3a, 0x20, 0x44, 0x49, 0x4d, 0x4d,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x50, 0x0f, 0x7a, 0xbb, 0x7c,
    0x23, 0x01, 0x00, 0x02, 0x00, 0x03, 0x02, 0x0b,
    0x5a, 0xfc, 0xd7, 0x17, 0x01, 0x00, 0x02, 0x00,
    0x05, 0x00, 0x0b, 0x00, 0x04, 0x00, 0x0d, 0x00,
    0x03, 0x02, 0x00, 0x00
};

////////////////////////////////////////////////////////////////////////////////
//  User Defined Data (errl/procedure callout)
static const eSEL::Params udPrClHeader{
    {"Section ID",        "0x5544 (UD)"},
    {"Section length",    "0x001c"},
    {"Section version",   "0x01"},
    {"Section subtype",   "0x06"},
    {"Section component", "errl"}
};
static const eSEL::Params udPrClPayload{
    {"Callout type",      "Procedure Callout"},
    {"Procedure",         "EPUB_PRC_HB_CODE"},
    {"Priority",          "SRCI_PRIORITY_HIGH"}
};
static const std::vector<uint8_t> udPrClData{
    /* Header */
    0x55, 0x44, 0x00, 0x1c, 0x01, 0x06, 0x01, 0x00,
    /* Payload */
    0x02, 0x54, 0x41, 0x4b, 0x00, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x00, 0x55, 0x00, 0x03, 0x06, 0x4c,
    0x00, 0x01, 0x00, 0x02
};

////////////////////////////////////////////////////////////////////////////////
//  User Defined Data (errl/hardware callout)
static const eSEL::Params udHwClHeader{
    {"Section ID",        "0x5544 (UD)"},
    {"Section length",    "0x0024"},
    {"Section version",   "0x01"},
    {"Section subtype",   "0x06"},
    {"Section component", "errl"}
};
static const eSEL::Params udHwClPayload{
    {"Callout type",      "Hardware Callout"},
    {"Target",            "Physical:/Sys0/Node0/DIMM73"},
    {"CPU id",            "0x0f"},
    {"Deconfig State",    "NO_DECONFIG"},
    {"GARD Error Type",   "GARD_NULL"},
    {"Priority",          "SRCI_PRIORITY_HIGH"}
};
static const std::vector<uint8_t> udHwClData{
    /* Header */
    0x55, 0x44, 0x00, 0x24, 0x01, 0x06, 0x01, 0x00,
    /* Payload */
    0x01, 0x54, 0x41, 0x4b, 0x00, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0f, 0x23, 0x01, 0x00, 0x02,
    0x00, 0x03, 0x49, 0x00
};

// clang-format on

/**
 * @brief Check for equalitu between readable parameters.
 *
 * @param[in] real - real parameters array
 * @param[in] expected - expected parameters array
 */
static void assertEqual(const eSEL::Params& real, const eSEL::Params& expected)
{
    bool equal = true;

    if (real.size() != expected.size())
    {
        std::cout << "Array size mismatch:" << std::endl;
        std::cout << " Real (" << real.size() << " items):" << std::endl;
        for (auto param : real)
            std::cout << "  " << param.name() << ": " << param.value()
                      << std::endl;
        std::cout << " Expected (" << expected.size()
                  << " items):" << std::endl;
        for (auto param : expected)
            std::cout << "  " << param.name() << ": " << param.value()
                      << std::endl;
        equal = false;
    }
    else
    {
        for (size_t i = 0; i < real.size(); ++i)
        {
            const eSEL::Param& r = real[i];
            const eSEL::Param& e = expected[i];

            if (r.name() != e.name())
            {
                std::cout << "Name mismatch at position " << i << std::endl;
                std::cout << "Real:     " << r.name() << std::endl;
                std::cout << "Expected: " << e.name() << std::endl;
                equal = false;
            }
            if (r.value() != e.value())
            {
                std::cout << "Value mismatch at position " << i << std::endl;
                std::cout << "Real:     " << r.value() << std::endl;
                std::cout << "Expected: " << e.value() << std::endl;
                equal = false;
            }
        }
    }

    if (!equal)
    {
        FAIL();
    }
}

/**
 * @brief Make eSEL buffer.
 *
 * @param[in] content - sections
 *
 * @return eSEL raw data
 */
static std::vector<uint8_t>
    makeSEL(std::initializer_list<const std::vector<uint8_t>> content)
{
    std::vector<uint8_t> sel;
    for (auto& it : content)
        sel.insert(sel.end(), it.begin(), it.end());

    // Set section counter inside the Private Header (always first section)
    eSEL::SectionPH::PHData* hdr = reinterpret_cast<eSEL::SectionPH::PHData*>(
        &sel[sizeof(eSEL::Section::Header)]);
    hdr->sectionCount = static_cast<uint8_t>(content.size());

    return sel;
}

/**
 * @brief Test section parser.
 *
 * @param[in] raw - section raw data
 * @param[in] header - expected header parameters
 * @param[in] payload - expected payload parameters
 */
template <typename T>
void testSection(const std::vector<uint8_t>& raw, const eSEL::Params& header,
                 const eSEL::Params& payload)
{
    // Construct section object
    eSEL::Section::Header hdr =
        *reinterpret_cast<const eSEL::Section::Header*>(raw.data());
    hdr.id = be16toh(hdr.id);
    hdr.length = be16toh(hdr.length);
    hdr.component = be16toh(hdr.component);
    const eSEL::Section::Payload pl(raw.begin() + sizeof(hdr), raw.end());
    T section(hdr, pl);

    // Test
    assertEqual(section.headerParams(), header);
    assertEqual(section.payloadParams(), payload);
}

TEST(ParserTest, SectionPH)
{
    testSection<eSEL::SectionPH>(phData, phHeader, phPayload);
}

TEST(ParserTest, SectionUH)
{
    testSection<eSEL::SectionUH>(uhData, uhHeader, uhPayload);
}

TEST(ParserTest, SectionPS)
{
    testSection<eSEL::SectionPS>(psData, psHeader, psPayload);
}

TEST(ParserTest, SectionUDString)
{
    testSection<eSEL::SectionUD>(udStrData, udStrHeader, udStrPayload);
}

TEST(ParserTest, SectionUDTarget)
{
    testSection<eSEL::SectionUD>(udTrgData, udTrgHeader, udTrgPayload);
}

TEST(ParserTest, SectionUDProcedureCallout)
{
    testSection<eSEL::SectionUD>(udPrClData, udPrClHeader, udPrClPayload);
}

TEST(ParserTest, SectionUDHardwareCallout)
{
    testSection<eSEL::SectionUD>(udHwClData, udHwClHeader, udHwClPayload);
}

TEST(ParserTest, Parse)
{
    // Construct and parse eSEL
    const std::vector<uint8_t> sel = makeSEL({
        phData,
        uhData,
        psData,
        udStrData,
        udTrgData,
        udPrClData,
        udHwClData,
    });
    eSEL::Event event;
    event.parse(sel.data(), sel.size());

    ASSERT_EQ(7, event.getSections().size());
}
