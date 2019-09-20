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

#pragma once

#include <map>

#include "errlplugins.H"

namespace eSEL
{

/**
 * @class PluginsFactory
 * @brief Plugins factory (singleton).
 */
template <typename T>
class PluginsFactory
{
  private:
    PluginsFactory<T>() = default;

  public:
    /**
     * @brief Get factory instance.
     *
     * @return factory instance
     */
    static PluginsFactory<T>& instance()
    {
        static PluginsFactory<T> factory;
        return factory;
    }

    /**
     * @brief Register plugin in the factory.
     *
     * @param[in] id - component ID
     * @param[in] fx - callback function
     */
    void add(comp_id_t id, typename T::func_t fx)
    {
        plugins_.emplace(std::make_pair(id, fx));
    }

    /**
     * @brief Deregister plugin.
     *
     * @param[in] id - component ID
     */
    void remove(comp_id_t id)
    {
        plugins_.erase(id);
    }

    /**
     * @brief Get plugin.
     *
     * @param[in] id - component ID
     *
     * @return callback parsing function
     */
    typename T::func_t get(comp_id_t id) const
    {
        auto it = plugins_.find(id);
        if (it != plugins_.end())
            return it->second;
        return nullptr;
    }

  private:
    /** @brief Registered plugins. */
    std::map<comp_id_t, typename T::func_t> plugins_;
};

/** @brief User Defined Data plugins factory. */
using DataPlugins = PluginsFactory<errl::DataPlugin>;
/** @brief SRC plugins factory. */
using SrcPlugins = PluginsFactory<errl::SrcPlugin>;

} // namespace eSEL
