/**
 * @brief eSEL parser console utility: entry point.
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
#include "task.hpp"

#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <setup.hpp>

// Constant flags for getopt.
// Used as id for long-only arguments.
enum
{
    OptBmcList = 0x0f00,
    OptPnorList,
    OptHbelDump,
    OptFspTrace,
    OptOCCStr,
    OptHbStr,
    OptHbSym
};

/** @brief Print title with version info. */
static void printTitle()
{
    std::cout << "OpenPOWER eSEL parser ver." << VERSION "." << std::endl;
    std::cout << "HostBoot plugin revision: " << HOSTBOOT_REVISION "."
              << std::endl;
}

/**
 * @brief Print help usage info.
 *
 * @param[in] app - application's file name
 */
static void printHelp(const char* app)
{
    printTitle();
    std::cout << "Copyright (c) 2019 YADRO.\n\n";
    std::cout << "Usage: " << app << " [OPTION...]\n\n";

    // clang-format off
    std::cout <<
    "Input options:\n"
    "  -f, --file=FILE    Parse and print eSEL from specified file\n"
    "  -b, --bmc=ID       Parse and print eSEL from BMC's event with specified ID\n"
    "      --bmc-list     Print list of BMC's events, that contain eSEL\n"
    "  -p, --pnor=NUM     Parse and print eSEL with specified number from PNOR flash\n"
    "      --pnor-list    Print list of events stored on PNOR flash\n"
    "      --hbel=FILE    Use file as HBEL partition instead of reading it from PNOR\n"
    "  -e, --ecc          Cut out ECC data from source file\n"
    "\n"
    "Output options:\n"
    "  -n, --number=NUM   Print only section with number NUM, this option  can be\n"
    "                     specified multiple times, not applicable for JSON output\n"
    "  -o, --output=FMT   Set output format:\n"
    "                       table  formatted output, used by default\n"
    "                       long   long lines without splitting\n"
    "                       json   JSON output\n"
    "                       hex    hex dump of payload\n"
    "                       bin    binary data of payload\n"
    "Parser setup:\n"
    "  --fsp-trace=FILE   Set path to FSP trace utility [" DEFAULT_FSP_TRACE "]\n"
    "  --occ-str=FILE     Set path to OCC string file [" DEFAULT_OCC_STRINGS "]\n"
    "  --hb-str=FILE      Set path to HostBoot string file [" DEFAULT_HB_STRINGS "]\n"
    "  --hb-sym=FILE      Set path to HostBoot symbols file [" DEFAULT_HB_SYMBOLS "]\n"
    "\n"
    "Other options:\n"
    "  -v, --version      Print version and exit\n"
    "  -h, --help         Print this help and exit\n";
    // clang-format on
}

/** @brief Application entry point. */
int main(int argc, char* argv[])
{
    int optFlag = 0;
    // clang-format off
    const struct option opts[] = {
        { "file",       required_argument, nullptr,  'f' },
        { "bmc",        required_argument, nullptr,  'b' },
        { "bmc-list",   no_argument,       &optFlag, OptBmcList },
        { "pnor",       required_argument, nullptr,  'p' },
        { "pnor-list",  no_argument,       &optFlag, OptPnorList },
        { "hbel",       required_argument, &optFlag, OptHbelDump },
        { "ecc",        no_argument,       nullptr,  'e' },
        { "number",     required_argument, nullptr,  'n' },
        { "output",     required_argument, nullptr,  'o' },
        { "fsp-trace",  required_argument, &optFlag, OptFspTrace },
        { "occ-str",    required_argument, &optFlag, OptOCCStr },
        { "hb-str",     required_argument, &optFlag, OptHbStr },
        { "hb-sym",     required_argument, &optFlag, OptHbSym },
        { "version",    no_argument,       nullptr,  'v' },
        { "help",       no_argument,       nullptr,  'h' },
        { 0, 0, nullptr, 0 }
    };
    // clang-format on

    // Short options
    char shortOpts[(sizeof(opts) / sizeof(opts[0])) * 2 + 1];
    char* shortOptPtr = shortOpts;
    for (size_t i = 0; i < sizeof(opts) / sizeof(opts[0]); ++i)
    {
        if (!opts[i].flag)
        {
            *shortOptPtr++ = opts[i].val;
            if (opts[i].has_arg != no_argument)
                *shortOptPtr++ = ':';
        }
    }

    Printer printer;
    Task task(printer);

    // Parse arguments
    opterr = 0;
    int optVal;
    while ((optVal = getopt_long(argc, argv, shortOpts, opts, nullptr)) != -1)
    {
        switch (optVal)
        {
            case 'f':
                task.fromFile(optarg);
                break;
            case 'b':
                try
                {
                    task.fromBmcEvent(std::stoul(optarg));
                }
                catch (std::exception&)
                {
                    std::cerr << "Invalid BMC event ID: " << optarg
                              << std::endl;
                    return EXIT_FAILURE;
                }
                break;
            case 'p':
                try
                {
                    task.fromPnorEvent(std::stoul(optarg));
                }
                catch (std::exception&)
                {
                    std::cerr << "Invalid PNOR event ID: " << optarg
                              << std::endl;
                    return EXIT_FAILURE;
                }
                break;
            case 'n':
                try
                {
                    printer.addFilter(std::stoul(optarg));
                }
                catch (std::exception&)
                {
                    std::cerr << "Invalid section number: " << optarg
                              << std::endl;
                    return EXIT_FAILURE;
                }
                break;
            case 'o':
                if (strcmp(optarg, "table") == 0)
                    printer.setFormat(Printer::Table);
                else if (strcmp(optarg, "long") == 0)
                    printer.setFormat(Printer::Long);
                else if (strcmp(optarg, "json") == 0)
                    printer.setFormat(Printer::Json);
                else if (strcmp(optarg, "hex") == 0)
                    printer.setFormat(Printer::Hex);
                else if (strcmp(optarg, "bin") == 0)
                    printer.setFormat(Printer::Bin);
                else
                {
                    std::cerr << "Invalid output format: " << optarg
                              << std::endl;
                    return EXIT_FAILURE;
                }
                break;
            case 'e':
                task.sourceWithEcc(true);
                break;
            case 'v':
                printTitle();
                return EXIT_SUCCESS;
            case 'h':
                printHelp(argv[0]);
                return EXIT_SUCCESS;
            case 0:
                // Long-only options
                switch (optFlag)
                {
                    case OptBmcList:
                        task.listBmcEvent();
                        break;
                    case OptPnorList:
                        task.listPnorEvent();
                        break;
                    case OptHbelDump:
                        task.pnorHbel(optarg);
                        break;
                    case OptFspTrace:
                        eSEL::setFspTrace(optarg);
                        break;
                    case OptOCCStr:
                        eSEL::setOccStrings(optarg);
                        break;
                    case OptHbStr:
                        eSEL::setHostbootStrings(optarg);
                        break;
                    case OptHbSym:
                        eSEL::setHostbootSymbols(optarg);
                        break;
                    default:
                        std::cerr << "Unsupported flag: " << optFlag
                                  << std::endl;
                        return EXIT_FAILURE;
                }
                break;
            default:
                std::cerr << "Invalid option: " << argv[optind - 1]
                          << std::endl;
                return EXIT_FAILURE;
        }
    }
    if (optind < argc)
    {
        std::cerr << "Invalid options: ";
        while (optind < argc)
            std::cerr << argv[optind++] << " ";
        std::cerr << std::endl;
        return EXIT_FAILURE;
    }

    return task.execute();
}
