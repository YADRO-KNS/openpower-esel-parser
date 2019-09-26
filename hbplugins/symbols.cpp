/**
 * @brief Implementation of hbSymbolTable interface.
 *        Unfortunately, Hostboot plugins have hardcoded paths to the symbol
 *        file (developer's host specific).
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

#include <symbols.H>

#include <fstream>
#include <map>
#include <string>

namespace eSEL
{

/**
 * @struct HostbootSymbol
 * @brief Hostboot symbol (function) description.
 */
struct HostbootSymbol
{
    uint32_t length;  ///< Length of function code
    std::string func; ///< Function signature
};

/** @brief Symbols map: address->description. */
static std::map<uint32_t, HostbootSymbol> HostbootSymbols;

/** @brief Path to the file with symbols list. */
static std::string HostbootSymbolsFile = DEFAULT_HB_SYMBOLS;

/* Symbol entry format:
F,400fea20,40107630,00000160,MVPD::mvpdRead(char*)
| |                 |        |
| |                 |        Function signature
| Start address     Length of function code
Entry type
*/
// Format specific constants
static constexpr char SymFunctionType = 'F';
static constexpr size_t SymAddressPos = 2;
static constexpr size_t SymAddressLen = 8;
static constexpr size_t SymLengthPos = 20;
static constexpr size_t SymLengthLen = 8;
static constexpr size_t SymSignaturePos = 29;

// Implementation of external interface (see setup.hpp)
void setHostbootSymbols(const char* symbolsFile)
{
    HostbootSymbolsFile = symbolsFile;
    HostbootSymbols.clear();
}

} // namespace eSEL

hbSymbolTable::hbSymbolTable()
{
}

hbSymbolTable::~hbSymbolTable()
{
}

// Read the symbols file.
// Called from hostboot/src/usr/errl/plugins/errludbacktrace.H
int hbSymbolTable::readSymbols(const char* path)
{
    if (!eSEL::HostbootSymbols.empty())
        return 0; // already loaded

    if (eSEL::HostbootSymbolsFile.empty())
        eSEL::HostbootSymbolsFile = path;

    try
    {
        std::ifstream symFile(eSEL::HostbootSymbolsFile);
        if (!symFile.good())
            throw std::exception();

        std::string line;
        while (std::getline(symFile, line))
        {
            if (line.length() < eSEL::SymSignaturePos ||
                line[0] != eSEL::SymFunctionType)
                continue; // skip non-function entries, see format description

            const uint32_t addr = std::stoul(
                line.substr(eSEL::SymAddressPos, eSEL::SymAddressLen), 0, 16);
            const uint32_t len = std::stoul(
                line.substr(eSEL::SymLengthPos, eSEL::SymLengthLen), 0, 16);
            const std::string func = line.substr(eSEL::SymSignaturePos);

            eSEL::HostbootSymbols.emplace(
                std::make_pair(addr, eSEL::HostbootSymbol{len, func}));
        }
    }
    catch (const std::exception&)
    {
        return -1;
    }

    return 0;
}

// Find and return the nearest symbol for the address given.
// Called from hostboot/src/usr/errl/plugins/errludbacktrace.H
char* hbSymbolTable::nearestSymbol(uint64_t address)
{
    auto it = eSEL::HostbootSymbols.lower_bound(address);
    if (it != eSEL::HostbootSymbols.end())
    {
        if (address != it->first && it != eSEL::HostbootSymbols.begin())
            --it; // move to the nearest entry
        if (address < it->first + it->second.length)
            return const_cast<char*>(it->second.func.c_str());
    }
    return nullptr;
}
