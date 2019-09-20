/**
 * @brief Plugins factory.
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

namespace errl
{

DataPlugin::DataPlugin(comp_id_t cid, func_t fx, errlCreator /*ssid*/) :
    cid_(cid)
{
    eSEL::DataPlugins::instance().add(cid_, fx);
}

DataPlugin::~DataPlugin()
{
    eSEL::DataPlugins::instance().remove(cid_);
}

SrcPlugin::SrcPlugin(comp_id_t cid, func_t fx, errlCreator /*ssid*/) : cid_(cid)
{
    eSEL::SrcPlugins::instance().add(cid_, fx);
}

SrcPlugin::~SrcPlugin()
{
    eSEL::SrcPlugins::instance().remove(cid_);
}

} // namespace errl
