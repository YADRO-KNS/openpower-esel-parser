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

#pragma once

#include <string>
#include <variant>
#include <vector>

namespace eSEL
{

/**
 * @class Param
 * @brief Typed section parameter (name : value).
 */
class Param
{
  public:
    /**
     * @enum Type
     * @brief General parameter types.
     */
    enum Type
    {
        Blank,   ///< Blank line: no name, no value
        Header,  ///< Header: no name, value is header's title
        Raw,     ///< Raw data string, name is empty (e.g. hex dump)
        Boolean, ///< Boolean type
        Numeric, ///< Unsigned numeric
        String   ///< String type
    };

    using variant_t =
        std::variant<bool, uint8_t, uint16_t, uint32_t, uint64_t, std::string>;

    /**
     * @brief Base constructor.
     *
     * @param[in] paramType - parameter type
     * @param[in] paramName - parameter name
     * @param[in] paramValue - parameter value
     */
    template <typename T>
    Param(Type paramType, const std::string& paramName, const T& paramValue) :
        type_(paramType), name_(paramName), value_(paramValue)
    {
    }

    /**
     * @brief Constructor for blank type.
     */
    Param();

    /**
     * @brief Constructor for heading type.
     *
     * @param[in] title - heading title
     */
    Param(const std::string& title);

    /**
     * @brief Constructor for boolean type.
     *
     * @param[in] paramName - parameter name
     * @param[in] paramValue - parameter value
     */
    Param(const std::string& paramName, bool paramValue);

    /**
     * @brief Constructor for string type.
     *
     * @param[in] paramName - parameter name
     * @param[in] paramValue - parameter value
     */
    Param(const std::string& paramName, const std::string& paramValue);

    /**
     * @brief Constructor for string type.
     *
     * @param[in] paramName - parameter name
     * @param[in] paramValue - parameter value
     */
    Param(const std::string& paramName, const char* paramValue);

    /**
     * @brief Constructor for numeric types.
     *
     * @param[in] paramName - parameter name
     * @param[in] paramValue - parameter value
     */
    template <typename T>
    Param(const std::string& paramName, T paramValue) :
        Param(Numeric, paramName, paramValue)
    {
    }

    /**
     * @brief Get general parameter type.
     *
     * @return parameter type
     */
    Type type() const;

    /**
     * @brief Get parameter name.
     *
     * @return parameter name
     */
    const std::string& name() const;

    /**
     * @brief Get parameter value as a string.
     *
     * @return text parameter value
     */
    std::string value() const;

    /**
     * @brief Get parameter value as raw variant.
     *
     * @return parameter value
     */
    const variant_t& variant() const;

  private:
    /** @brief Parameter type. */
    Type type_;
    /** @brief Parameter name. */
    std::string name_;
    /** @brief Parameter value. */
    variant_t value_;
};

/** @brief Section parameters array. */
using Params = std::vector<Param>;

} // namespace eSEL
