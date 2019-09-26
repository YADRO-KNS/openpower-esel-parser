/**
 * @brief eSEL printer.
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

#include <event.hpp>
#include <set>

/**
 * @class Printer
 * @brief eSEL event printer.
 */
class Printer
{
  public:
    /**
     * @enum Format
     * @brief Output format types.
     */
    enum Format
    {
        Table, ///< Table view (default)
        Long,  ///< Long lines
        Json,  ///< JSON format
        Hex,   ///< Hex dump
        Bin    ///< Binary data of payload
    };

    /**
     * @brief Constructor.
     */
    Printer();

    /**
     * @brief Set output format.
     *
     * @param[in] fmt - format to set
     */
    void setFormat(Format fmt);

    /**
     * @brief Add section number to filter.
     *
     * @param[in] num - section number
     */
    void addFilter(size_t num);

    /**
     * @brief Print eSEL content to standart output.
     *
     * @param[in] eSEL - eSEL event
     */
    void print(const eSEL::Event& event) const;

  private:
    /**
     * @brief Print eSEL content in binary format (payload only).
     *
     * @param[in] eSEL - eSEL event
     */
    void printEventBin(const eSEL::Event& event) const;

    /**
     * @brief Print eSEL content in JSON format.
     *
     * @param[in] eSEL - eSEL event
     */
    void printEventJson(const eSEL::Event& event) const;

    /**
     * @brief Print parameters in JSON format.
     *
     * @param[in] indent - number of spaces used for indentation
     * @param[in] params - parameters array to print
     */
    void printParametersJson(size_t indent, const eSEL::Params& params) const;

    /**
     * @brief Escape JSON string.
     *
     * @param[in] original - original text to escape
     *
     * @return escaped string
     */
    std::string jsonEscape(const std::string& original) const;

    /**
     * @brief Print eSEL content in text/hex format.
     *
     * @param[in] eSEL - eSEL event
     */
    void printEventText(const eSEL::Event& event) const;

    /**
     * @brief Print parameters as text.
     *
     * @param[in] params - parameters array to print
     */
    void printParametersText(const eSEL::Params& params) const;

    using Line = std::pair<size_t, size_t>;

    /**
     * @brief Smart split string to short parts.
     *
     * @param[in] text - source string
     * @param[in] width - maximum part size in symbols
     *
     * @return array of <position, length>
     */
    std::vector<Line> split(const std::string& text, size_t width) const;

  private:
    /** @brief Output format. */
    Format format_;
    /** @brief Maximum size of output line (number of terminal columns). */
    size_t maxColumns_;
    /** @brief Array with sections number to print, empty for all. */
    std::set<size_t> sectionFilter_;
};
