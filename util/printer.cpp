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

#include "printer.hpp"

#include <sys/ioctl.h>
#include <unistd.h>

#include <cstdio>
#include <hexdump.hpp>
#include <iostream>

/** @brief Number of symbols reserved for 'name' field. */
constexpr int NameWidth = 22;
/** @brief Standart width for most terminals. */
constexpr size_t CommonWidth = 80;

Printer::Printer() : maxColumns_(CommonWidth)
{
    setFormat(Table);
}

void Printer::setFormat(Format fmt)
{
    format_ = fmt;

    if (format_ == Table)
    {
        // If stdout is not a pipe - get terminal width
        if (isatty(STDIN_FILENO) == 1)
        {
            winsize wsz;
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsz) != -1)
            {
                maxColumns_ =
                    wsz.ws_col < CommonWidth ? wsz.ws_col : CommonWidth;
            }
        }
    }
}

void Printer::addFilter(size_t num)
{
    sectionFilter_.insert(num);
}

void Printer::print(const eSEL::Event& event) const
{
    switch (format_)
    {
        case Table:
        case Long:
        case Hex:
            printEventText(event);
            break;
        case Json:
            printEventJson(event);
            break;
        case Bin:
            printEventBin(event);
            break;
    }
}

void Printer::printEventBin(const eSEL::Event& event) const
{
    const eSEL::Sections& sections = event.getSections();
    const size_t total = sections.size();
    for (size_t i = 0; i < total; ++i)
    {
        if (!sectionFilter_.empty() &&
            sectionFilter_.find(i + 1) == sectionFilter_.end())
            continue; // Skip by filter

        const eSEL::Section::Payload& pl = sections[i]->payload();
        fwrite(pl.data(), 1, pl.size(), stdout);
    }
}

void Printer::printEventJson(const eSEL::Event& event) const
{
    std::cout << "{\n";

    // Print SEL record
    std::optional<eSEL::SelRecord> sel = event.getSelRecord();
    if (sel)
    {
        std::cout << "  \"sel\": [\n";
        printParametersJson(4, sel->params());
        std::cout << "  ],\n";
    }

    // Print event's sections
    std::cout << "  \"sections\": [\n";
    const eSEL::Sections& sections = event.getSections();
    const size_t total = sections.size();
    for (size_t i = 0; i < total; ++i)
    {
        std::cout << "    {\n";

        // Header
        std::cout << "      \"header\": [\n";
        printParametersJson(8, sections[i]->headerParams());
        std::cout << "      ],\n";

        // Parameters
        std::cout << "      \"params\": [\n";
        printParametersJson(8, sections[i]->payloadParams());
        std::cout << "      ]\n";

        std::cout << "    }";
        if (i != total - 1)
            std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "  ]\n";

    std::cout << "}\n";
}

void Printer::printParametersJson(size_t indent,
                                  const eSEL::Params& params) const
{
    const std::string spaces(indent, ' ');

    const size_t total = params.size();
    for (size_t i = 0; i < total; ++i)
    {
        const eSEL::Param& param = params[i];

        switch (param.type())
        {
            case eSEL::Param::Blank:
                continue;
            case eSEL::Param::Header:
                std::cout << spaces << "[\"header\", "
                          << jsonEscape(param.value()) << "]";
                break;
            case eSEL::Param::Raw:
                std::cout << spaces << "[\"raw\", " << jsonEscape(param.value())
                          << "]";
                break;
            case eSEL::Param::Boolean:
                std::cout << spaces << "[" << jsonEscape(param.name()) << ", "
                          << (std::get<bool>(param.variant()) ? "true"
                                                              : "false")
                          << "]";
                break;
            case eSEL::Param::String:
                std::cout << spaces << "[" << jsonEscape(param.name()) << ", "
                          << jsonEscape(param.value()) << "]";
                break;
            case eSEL::Param::Numeric:
                std::cout << spaces << "[" << jsonEscape(param.name()) << ", ";
                std::visit(
                    [](auto&& arg) {
                        using T = std::decay_t<decltype(arg)>;
                        if constexpr (std::is_arithmetic<T>::value)
                            std::cout << std::to_string(arg);
                        else
                            std::cout << "null";
                    },
                    param.variant());
                std::cout << "]";
                break;
        }
        if (i != total - 1)
            std::cout << ",";
        std::cout << "\n";
    }
}

std::string Printer::jsonEscape(const std::string& original) const
{
    std::string out;
    out.reserve(original.length() + 2 /*quotes*/ + 16 /*reserve for escape*/);
    out = "\"";

    for (char ch : original)
    {
        switch (ch)
        {
            case '\\':
                out += '\\';
                out += ch;
                break;
            case '/':
                out += '\\';
                out += ch;
                break;
            case '"':
                out += '\\';
                out += ch;
                break;
            case '\b':
                out += "\\b";
                break;
            case '\t':
                out += "\\t";
                break;
            case '\f':
                out += "\\f";
                break;
            case '\r':
                out += "\\r";
                break;
            case '\n':
                out += "\\n";
                break;
            default:
                out += ch;
        }
    }

    out += '"';
    return out;
}

void Printer::printEventText(const eSEL::Event& event) const
{
    static const std::string HeaderLine(maxColumns_, '=');

    // Print SEL record
    if (format_ != Hex)
    {
        std::optional<eSEL::SelRecord> sel = event.getSelRecord();
        if (sel)
        {
            std::cout << HeaderLine << "\n";
            std::cout << "System Event Log (SEL) record\n";
            std::cout << HeaderLine << "\n";
            printParametersText(sel->params());
            std::cout << "\n";
        }
    }

    // Print event's sections
    const eSEL::Sections& sections = event.getSections();
    const size_t total = sections.size();
    for (size_t i = 0; i < total; ++i)
    {
        if (!sectionFilter_.empty() &&
            sectionFilter_.find(i + 1) == sectionFilter_.end())
            continue; // Skip by filter

        // Header
        std::cout << HeaderLine << "\n";
        std::cout << "Section " << (i + 1) << " of " << total << ": "
                  << sections[i]->name() << "\n";
        std::cout << HeaderLine << "\n";
        printParametersText(sections[i]->headerParams());

        // Payload
        if (format_ != Hex)
        {
            printParametersText(sections[i]->payloadParams());
        }
        else
        {
            const eSEL::Section::Payload& pl = sections[i]->payload();
            std::cout << eSEL::hexDump(pl.data(), pl.size()) << "\n";
        }

        std::cout << "\n"; // Blank line between sections
    }
}

void Printer::printParametersText(const eSEL::Params& params) const
{
    for (const auto& param : params)
    {
        switch (param.type())
        {
            case eSEL::Param::Blank:
                std::cout << "\n";
                break;
            case eSEL::Param::Header:
                if (format_ == Long)
                {
                    std::cout << param.value() << "\n";
                }
                else
                {
                    const size_t centered =
                        param.value().length() > maxColumns_
                            ? 0
                            : maxColumns_ / 2 - param.value().length() / 2;
                    std::cout << std::string(centered, ' ') << param.value()
                              << "\n";
                }
                break;
            case eSEL::Param::Raw:
                if (format_ == Long)
                {
                    std::cout << param.value() << "\n";
                }
                else
                {
                    for (auto pos : split(param.value(), maxColumns_))
                    {
                        std::cout << param.value().substr(pos.first, pos.second)
                                  << "\n";
                    }
                }
                break;
            default:
                if (!param.name().empty())
                    std::cout << param.name() << ":";
                if (param.value().empty())
                {
                    std::cout << "\n";
                    continue;
                }
                if (param.name().empty())
                    std::cout << param.name() << " ";
                if (NameWidth > param.name().length())
                {
                    std::cout
                        << std::string(NameWidth - param.name().length(), ' ');
                }
                if (format_ == Long)
                    std::cout << param.value() << "\n";
                else
                {
                    const int leftIndent = NameWidth + 1 /* delimiter ':' */;
                    for (auto pos :
                         split(param.value(), maxColumns_ - leftIndent))
                    {
                        if (pos.first)
                            std::cout << std::string(leftIndent, ' ');
                        std::cout << param.value().substr(pos.first, pos.second)
                                  << "\n";
                    }
                }
        }
    }
}

std::vector<Printer::Line> Printer::split(const std::string& text,
                                          size_t width) const
{
    std::vector<Line> positions;

    const size_t textLen = text.length();
    positions.reserve(1 + textLen / width);

    size_t start = 0;
    while (start < textLen)
    {
        // Calculate maximum line length
        size_t len = width;
        if (start + len > textLen)
            len = textLen - start;

        // Check for forced line feed
        size_t lf = start;
        while (lf < start + len && text[lf] != '\n')
            ++lf;
        if (lf < start + len)
            len = lf - start;

        // Search for the nearest space to break the line
        if (len == width &&
            start + len < textLen) // skip searching if it is the last line
        {
            size_t space = start + len;
            while (space != start && !isspace(text[space]))
                --space;
            // Don't brake if space too far
            if (space > start + width / 2)
                len = space - start;
        }

        positions.emplace_back(Line{start, len});
        start += len;

        // Skip spaces
        while (start < textLen && isspace(text[start]))
            ++start;
    }

    if (positions.empty())
    {
        // Empty value
        positions.emplace_back(Line{0, 0});
    }

    return positions;
}
