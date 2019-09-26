/**
 * @brief FSP Trace plugins.
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

#include "errlplugins.hpp"

#include <hbotcompid.H>
#include <unistd.h>

#include <climits>
#include <cstdio>
#include <string>
#include <system_error>
#include <vector>

namespace eSEL
{

/** @brief Path to Hostboot string file */
static std::string HBStringFile = DEFAULT_HB_STRINGS;
/** @brief Path to OCC string file */
static std::string OCCStringFile = DEFAULT_OCC_STRINGS;
/** @brief Path to FSP trace utility */
static std::string FspUtil = DEFAULT_FSP_TRACE;

/**
 * @class TempFile
 * @brief Temporary file.
 */
class TempFile
{
  public:
    TempFile() : fd_(-1)
    {
    }

    ~TempFile()
    {
        close();
    }

    /**
     * @brief Create file.
     */
    void create()
    {
        // Create temporary file
        char tmpTemplate[] = "/tmp/fsptrace_XXXXXX";
        fd_ = mkstemp(tmpTemplate);
        if (fd_ == -1)
        {
            throw std::system_error(errno, std::system_category(),
                                    "Unable to create temporary file");
        }

        // Get absolute path to the temporary file
        std::string link = std::string("/proc/self/fd/") + std::to_string(fd_);
        char path[PATH_MAX];
        const ssize_t len = readlink(link.c_str(), path, sizeof(path));
        if (len == -1)
        {
            throw std::system_error(errno, std::system_category(),
                                    "Unable to get temporary file name");
        }
        path_.assign(path, path + len);
    }

    /**
     * @brief Write data to temporary file.
     *
     * @param[in] buffer - source data buffer
     * @param[in] len - length of the buffer in bytes
     */
    void write(const uint8_t* buffer, size_t len)
    {
        size_t pos = 0;
        while (pos < len)
        {
            const ssize_t written = ::write(fd_, buffer + pos, len - pos);
            if (written == -1)
            {
                throw std::system_error(errno, std::system_category(),
                                        "Unable to write temporary file");
            }
            pos += written;
        }
    }

    /**
     * @brief Close file.
     */
    void close()
    {
        if (fd_ != -1)
        {
            ::close(fd_);
            fd_ = -1;
            if (!path_.empty())
            {
                remove(path_.c_str());
                path_.clear();
            }
        }
    }

    /**
     * @brief Get absolute path to the file.
     *
     * @return path
     */
    const std::string& path() const
    {
        return path_;
    }

  private:
    /** @brief File descriptor. */
    int fd_;
    /** @brief Absolute path to the file. */
    std::string path_;
};

/**
 * @brief Get trace.
 *
 * @param[in] cb - callback for user parser (param collector)
 * @param[in] buffer - raw log buffer
 * @param[in] len - length of the buffer
 * @param[in] stringFile - string file
 *
 * @return operation status, true if success
 */
static bool fspTrace(ErrlUsrParser& cb, const uint8_t* buffer, size_t len,
                     const std::string& stringFile)
{
    std::string trace;

    cb.PrintString("FSP trace utility", FspUtil.c_str());
    cb.PrintString("String file", stringFile.c_str());

    try
    {
        // We have to create a real file to pass it to FSP trace
        TempFile logFile;
        logFile.create();
        logFile.write(buffer, len);

        // Command to run FSP trace
        std::string traceCommand = FspUtil;
        traceCommand += " --stringfile \"" + stringFile + '\"';
        traceCommand += " \"" + logFile.path() + '\"';
        traceCommand += " 2>&1";

        // Run FSP trace and read output
        FILE* pipeOut = popen(traceCommand.c_str(), "r");
        if (!pipeOut)
        {
            throw std::system_error(errno, std::system_category(),
                                    "Unable to open pipe");
        }

        char pipeData[128]; // max line size, that fsp-trace prints
        while (!feof(pipeOut))
        {
            const size_t rb = fread(pipeData, 1, sizeof(pipeData), pipeOut);
            if (rb)
                trace.append(pipeData, pipeData + rb);
        }
        const int rc = pclose(pipeOut);
        if (rc)
        {
            throw std::system_error(rc, std::system_category(),
                                    "Wrong FSP exit code");
        }
        cb.PrintTrace(trace.c_str());
    }
    catch (const std::exception& e)
    {
        cb.PrintString("Error", e.what());
        if (!trace.empty())
            cb.PrintString("Output message", trace.c_str());
        return false;
    }

    return true;
}

// Hostboot trace plugin function, see errl::DataPlugin::func_t for more info.
static bool HBTrace(ErrlUsrParser& cb, void* buffer, uint32_t len,
                    errlver_t /*ver*/, errlsubsec_t sst)
{
    if (sst != FIPS_ERRL_UDT_HB_TRACE)
        return false;
    return fspTrace(cb, reinterpret_cast<const uint8_t*>(buffer), len,
                    HBStringFile);
}

// Hostboot trace plugin registration
static errl::DataPlugin hbTracePlugin(FIPS_ERRL_COMP_ID, HBTrace, 0);

// OCC trace plugin function, see errl::DataPlugin::func_t for more info.
static bool OCCTrace(ErrlUsrParser& cb, void* buffer, uint32_t len,
                     errlver_t /*ver*/, errlsubsec_t sst)
{
    if (sst != 0 /* OCC trace subtype */)
        return false;

    // OCC adds a header, just skip it. See OCC src for details.
    const uint32_t occHdrSize = 0x7c;
    if (len < occHdrSize)
        return false;

    return fspTrace(cb, reinterpret_cast<const uint8_t*>(buffer) + occHdrSize,
                    len - occHdrSize, OCCStringFile);
}

// Hostboot trace plugin registration
static errl::DataPlugin occTracePlugin(OCCC_COMP_ID, OCCTrace, 0);

// Implementation of external interface (see setup.hpp)
void setHostbootStrings(const char* stringFile)
{
    HBStringFile = stringFile;
}

// Implementation of external interface (see setup.hpp)
void setOccStrings(const char* stringFile)
{
    OCCStringFile = stringFile;
}

// Implementation of external interface (see setup.hpp)
void setFspTrace(const char* path)
{
    FspUtil = path;
}

} // namespace eSEL
