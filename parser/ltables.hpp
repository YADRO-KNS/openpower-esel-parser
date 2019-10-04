/**
 * @brief Lookup tables: represents an ID as a human readable values.
 * Most of the data comes from the following projects:
 * 1. https://github.com/open-power/errl
 * 2. https://github.com/open-power/libopalevents
 * 3. https://github.com/open-power/hostboot
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

#include "hexdump.hpp"

#include <map>

namespace eSEL
{

/**
 * @class LookupTable
 * @brief Map of ID->Text pairs.
 */
template <typename T>
class LookupTable : public std::map<T, const char*>
{
  public:
    using std::map<T, const char*>::map;

    /**
     * @brief Get value from the table.
     *
     * @param[in] key - key to search
     * @param[in] defValue - default value used if key not found
     *
     * @return text value
     */
    std::string get(T key, const char* defValue = "Unknown") const
    {
        std::string val;

        auto it = this->find(key);
        if (it != this->end())
            val = it->second;
        else if (defValue)
        {
            val = defValue;
            val += " (";
            val += toHex(key);
            val += ')';
        }

        return val;
    }
};

/** @brief Subsystems names. */
extern const LookupTable<uint8_t> SubsystemName;
/** @brief Creator subsystem types. */
extern const LookupTable<uint8_t> CreatorSubSys;
/** @brief Event severities. */
extern const LookupTable<uint8_t> EventSeverity;
/** @brief Event scopes. */
extern const LookupTable<uint8_t> EventScope;
/** @brief Event types. */
extern const LookupTable<uint8_t> EventType;

} // namespace eSEL
