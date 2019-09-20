/**
 * @brief Section parameter as a human readable text pair (name : value).
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

#include "param.hpp"

#include "hexdump.hpp"

#include <algorithm>

namespace eSEL
{

Param::Param() : Param(Blank, std::string(), 0U)
{
}

Param::Param(const std::string& title) : Param(Header, std::string(), title)
{
}

Param::Param(const std::string& paramName, bool paramValue) :
    Param(Boolean, paramName, paramValue)
{
}

Param::Param(const std::string& paramName, const std::string& paramValue) :
    Param(String, paramName, paramValue)
{
}

Param::Param(const std::string& paramName, const char* paramValue) :
    Param(String, paramName, std::string(paramValue ? paramValue : ""))
{
    // Trim spaces from end of parameter value
    std::string& val = std::get<std::string>(value_);
    val.erase(std::find_if(val.rbegin(), val.rend(),
                           [](int ch) { return !std::isspace(ch); })
                  .base(),
              val.end());
}

Param::Type Param::type() const
{
    return type_;
}

const std::string& Param::name() const
{
    return name_;
}

std::string Param::value() const
{
    std::string val;

    if (type_ != Blank)
    {
        std::visit(
            [&val](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, bool>)
                    val = arg ? "True" : "False";
                else if constexpr (std::is_arithmetic<T>::value)
                    val = toHex(arg);
                else if constexpr (std::is_same_v<T, std::string>)
                    val = arg;
                else
                    static_assert(T::value, "Unhandled value type");
            },
            value_);
    }

    return val;
}

const Param::variant_t& Param::variant() const
{
    return value_;
}

} // namespace eSEL
