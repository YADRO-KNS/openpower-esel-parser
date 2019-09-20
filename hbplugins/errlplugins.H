/**
 * @brief Plugins interface declaration.
 *        More info: https://github.com/open-power/errl
 *                   file errl/include/errlplugins.H
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
#include <hberrltypes.H>
#include <sys/sync.h>

#include "errlusrparser.H"
#include "srcisrc.H"

// Prevent warning: "TO_UINT##" redefined
#undef TO_UINT16
#undef TO_UINT32
#undef TO_UINT64

// Prevent error: "ERRL_CID_HOSTBOOT" was not declared in this scope
using namespace ERRORLOG;

typedef uint8_t errlver_t;
typedef uint8_t errlsubsec_t;
typedef uint16_t comp_id_t;
typedef uint8_t errlCreator;

namespace errl
{

/**
 * @class DataPlugin
 * @brief Plugin for parsing User Defined Data section.
 */
class DataPlugin
{
  public:
    /**
     * @brief Parsing function declaration.
     *
     * @param[in] cb - callback for user parser (param collector)
     * @param[in] buffer - source data buffer to parser
     * @param[in] len - length of the buffer in bytes
     * @param[in] ver - section version
     * @param[in] sst - section subsystem ID
     *
     * @return true if parsing completed successfully
     */
    typedef bool (*func_t)(ErrlUsrParser& cb, void* buffer, uint32_t len,
                           errlver_t ver, errlsubsec_t sst);

    /**
     *  @brief Constructor: register the plugin.
     *
     *  @param[in] cid - component ID
     *  @param[in] fx - plugin entry point to be registered
     *  @param[in] ssid - creator subsystem ID (unused)
     */
    DataPlugin(comp_id_t cid, func_t fx, errlCreator ssid);

    /**
     *  @brief Destructor: deregister the plugin.
     */
    virtual ~DataPlugin();

  private:
    /** @brief Component ID. */
    comp_id_t cid_;
};

/**
 * @class SrcPlugin
 * @brief Event description from source code.
 */
class SrcPlugin
{
  public:
    /**
     * @brief Parsing function declaration.
     *
     * @param[in] cb - callback for user parser (param collector)
     * @param[in] src - source descriptor
     *
     * @return true if parsing completed successfully
     */
    typedef bool (*func_t)(ErrlUsrParser& cb, const SrciSrc& src);

    /**
     * @brief Constructor: register the plugin.
     *
     * @param[in] cid - component ID
     * @param[in] fx - plugin entry point to be registered
     * @param[in] ssid - creator subsystem ID (unused)
     */
    SrcPlugin(comp_id_t cid, func_t fx, errlCreator ssid);

    /**
     * @brief Destructor: deregister the plugin.
     */
    virtual ~SrcPlugin();

  private:
    /** @brief Component ID. */
    comp_id_t cid_;
};

} // namespace errl
