/**
 * @brief High level functionality of the console utility.
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

#include "task.hpp"

#include <endian.h>
#include <sys/stat.h>

#include <algorithm>
#include <filesystem>
#include <fmtexcept.hpp>
#include <fstream>
#include <hexdump.hpp>
#include <iomanip>
#include <iostream>
#include <section_ph.hpp>

/** @brief Size of HBEL partition on PNOR. */
static constexpr int HbelPartitionSize = 0x24000;
/** @brief Size of single event inside HBEL partition of PNOR. */
static constexpr int HbelEventSize = 4096;
/** @brief Path to BMC events. */
static const char* BmcEventPath = "/var/lib/phosphor-logging/errors";
/** @brief Token to search eSEL data inside BMC event. */
static const uint8_t BmcEselToken[] = {'E', 'S', 'E', 'L', '='};

Task::Task(const Printer& printer) :
    action_(PrintSEL), printer_(printer), pelFile_(nullptr),
    bmcEventId_(std::string::npos), pnorEventId_(std::string::npos),
    eccExist_(false), hbelFile_(nullptr)
{
}

void Task::fromFile(const char* path)
{
    pelFile_ = path;
}

void Task::fromBmcEvent(size_t eventId)
{
    bmcEventId_ = eventId;
}

void Task::fromPnorEvent(size_t eventId)
{
    pnorEventId_ = eventId;
    eccExist_ = true;
}

void Task::listBmcEvent()
{
    action_ = PrintBmcList;
}

void Task::listPnorEvent()
{
    action_ = PrintPnorList;
    eccExist_ = true;
}

void Task::sourceWithEcc(bool eccExist)
{
    eccExist_ = eccExist;
}

void Task::pnorHbel(const char* path)
{
    hbelFile_ = path;
}

int Task::execute()
{
    int rc = EXIT_SUCCESS;

    try
    {
        switch (action_)
        {
            case PrintSEL:
                printEvent();
                break;
            case PrintBmcList:
                printBmcEventList();
                break;
            case PrintPnorList:
                printPnorEventList();
                break;
        }
    }
    catch (const eSEL::InvalidFormat& e)
    {
        std::cerr << "Invalid eSEL format: " << e.what() << std::endl;
        rc = EXIT_FAILURE;
    }
    catch (const std::system_error& e)
    {
        std::cerr << e.what() << std::endl;
        rc = e.code().value();
        if (!rc)
            rc = EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        rc = EXIT_FAILURE;
    }

    return rc;
}

void Task::printEvent() const
{
    std::vector<uint8_t> data;

    if (pelFile_)
    {
        data = readFile(pelFile_);
        if (eccExist_)
            data = removeEcc(data);
    }
    else if (bmcEventId_ != std::string::npos)
        data = readBmcEvent();
    else if (pnorEventId_ != std::string::npos)
        data = readPnorEvent();
    else
        throw std::runtime_error("Undefined eSEL source to read, exiting.");

    eSEL::Event event;
    try
    {
        event.parse(data.data(), data.size());
    }
    catch (const eSEL::InvalidFormat& e)
    {
        if (event.getSections().empty())
            throw;
        // Show warning and print the parsed part of the event
        std::cerr << "Invalid eSEL format: " << e.what() << std::endl;
    }
    printer_.print(event);
}

std::vector<uint8_t> Task::readFile(const char* path) const
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file)
        throw std::system_error(errno, std::system_category(), path);

    std::vector<uint8_t> content;

    std::copy(std::istreambuf_iterator<char>(file),
              std::istreambuf_iterator<char>(), std::back_inserter(content));

    return content;
}

std::vector<uint8_t> Task::removeEcc(const std::vector<uint8_t>& data) const
{
    std::vector<uint8_t> result;

    const size_t len = data.size();
    result.reserve(len - len / 9);
    size_t i = 0;
    for (uint8_t n : data)
    {
        if (++i == 9)
            i = 0;
        else
            result.push_back(n);
    }

    return result;
}

std::vector<uint8_t> Task::readBmcEvent() const
{
    std::vector<uint8_t> eselRaw;

    // Read BMC event
    const std::string eventFile =
        BmcEventPath + std::string("/") + std::to_string(bmcEventId_);
    const std::vector<uint8_t> eventData = readFile(eventFile.c_str());

    auto it = std::search(eventData.begin(), eventData.end(), BmcEselToken,
                          BmcEselToken + sizeof(BmcEselToken));
    if (it != eventData.end())
    {
        eselRaw.reserve(2048); // average size of eSEL

        // Cut out ESEL property value and convert it from hex dump to binary
        const uint8_t* eselPos = &*it + sizeof(BmcEselToken);
        while (*eselPos)
        {
            // Skip spaces
            while (isspace(*eselPos))
                ++eselPos;
            if (!isxdigit(*eselPos))
                break; // end of hex dump

            // Convert 2-bytes text hex to numeric
            uint8_t val = 0;
            for (uint8_t hb = 0; hb <= 1; ++hb)
            {
                val <<= 4 * hb;
                if (*eselPos >= '0' && *eselPos <= '9')
                    val |= *eselPos - '0';
                else if (*eselPos >= 'a' && *eselPos <= 'f')
                    val |= 0x0a + *eselPos - 'a';
                else if (*eselPos >= 'A' && *eselPos <= 'F')
                    val |= 0x0a + *eselPos - 'A';
                else
                    throw std::runtime_error("Invalid hex format");
                ++eselPos;
            }
            eselRaw.push_back(val);
        }
    }

    if (eselRaw.empty())
    {
        throw std::runtime_error(std::string("BMC event with ID ") +
                                 std::to_string(bmcEventId_) +
                                 " doesn't contain eSEL entry");
    }

    return eselRaw;
}

void Task::printBmcEventList() const
{
    std::map<int, std::string> events;
    for (auto& pathIt : std::filesystem::directory_iterator(BmcEventPath))
    {
        const std::filesystem::path& eventPath = pathIt.path();
        const int id = atoi(eventPath.filename().c_str());
        if (!id)
            continue;

        // Read BMC event
        const std::vector<uint8_t> eventData = readFile(eventPath.c_str());
        // Check for ESEL property
        auto it = std::search(eventData.begin(), eventData.end(), BmcEselToken,
                              BmcEselToken + sizeof(BmcEselToken));
        if (it != eventData.end())
        {
            // Get file timestamp, it will be used as a description
            std::string timestamp;
            struct stat s;
            struct tm t;
            if (!stat(eventPath.c_str(), &s) && localtime_r(&s.st_mtime, &t))
            {
                char buf[32]; // enough for "%x %X"
                strftime(buf, sizeof(buf), "%x %X", &t);
                timestamp = buf;
            }
            events.insert(std::make_pair(id, timestamp));
        }
    }

    // Print event list
    if (events.empty())
        std::cout << "BMC doesn't contain event records with ESEL.\n";
    else
    {
        std::cout << "ID    Event file time stamp\n";
        std::cout << "----  ---------------------\n";
        for (auto event : events)
        {
            std::cout << std::setw(4) << event.first << "  " << event.second
                      << std::endl;
        }
    }
}

std::vector<uint8_t> Task::readHbel() const
{
    /* Unfortunately, it is impossible to use pflash library (libflash.so)
       to read the partition. Pflash's SDK (include headers) depends on
       skiboot source code and you can not build third party modules outside
       the skiboot tree. So, the partition will be read using console pflash
       utility instead of calling libflash.so. */
    std::vector<uint8_t> data;
    data.reserve(HbelPartitionSize);

    if (hbelFile_)
        data = readFile(hbelFile_);
    else
    {
        // Run pflash to read HBEL partition
        FILE* pipeOut = popen(
            "/usr/sbin/pflash -P HBEL -r /dev/stderr 2>&1 >/dev/null", "r");
        if (!pipeOut)
            throw std::system_error(errno, std::system_category(),
                                    "Unable to open pflash pipe");

        // Read via pipe
        char pipeData[512]; // arbitrary size
        while (!feof(pipeOut))
        {
            const size_t rb = fread(pipeData, 1, sizeof(pipeData), pipeOut);
            if (rb)
            {
                std::copy(pipeData, pipeData + rb, std::back_inserter(data));
            }
        }

        // Check exit code
        const int rc = pclose(pipeOut);
        if (rc)
        {
            throw std::system_error(rc, std::system_category(),
                                    "Failed to read HBEL partition");
        }
    }

    return removeEcc(data);
}

void Task::printPnorEventList() const
{
    std::vector<uint32_t> events;

    // Read HBEL and construct event list
    const std::vector<uint8_t> data = readHbel();
    for (size_t i = 0; (i + 1) * HbelEventSize < data.size(); ++i)
    {
        const uint8_t* eventStart = &data[i * HbelEventSize];

        // Check Private Header section existing
        const uint16_t sid = *reinterpret_cast<const uint16_t*>(eventStart);
        if (be16toh(sid) != eSEL::SectionPH::SectionId)
            break;
        // Use log entry id as a description
        const eSEL::SectionPH::PHData& ph =
            *reinterpret_cast<const eSEL::SectionPH::PHData*>(
                eventStart + sizeof(eSEL::Section::Header));
        events.push_back(be32toh(ph.logEntryId));
    }

    // Print event list
    if (events.empty())
        std::cout << "HBEL partition of PNOR doesn't contain event records.\n";
    else
    {
        std::cout << "ID  Log Entry ID\n";
        std::cout << "--  ------------\n";
        for (size_t i = 0; i < events.size(); ++i)
        {
            std::cout << std::dec << std::setfill(' ') << std::setw(2) << i
                      << "  0x" << std::hex << std::setfill('0') << std::setw(8)
                      << events[i] << std::endl;
        }
    }
}

std::vector<uint8_t> Task::readPnorEvent() const
{
    const std::vector<uint8_t> data = readHbel();
    if ((pnorEventId_ + 1) * HbelEventSize > data.size())
    {
        throw std::runtime_error(std::string("Event with ID ") +
                                 std::to_string(pnorEventId_) +
                                 " not found in HBEL partition");
    }
    const uint8_t* eventStart = &data[pnorEventId_ * HbelEventSize];
    return std::vector(eventStart, eventStart + HbelEventSize);
}
