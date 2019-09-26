# OpenPOWER eSEL parser
eSEL parser is a library and console utility used for parsing OpenPOWER
Extended System Event Log, which consist of an SEL record header and Platform
Error Log (PEL) entry.
The parser uses [HostBoot's](https://github.com/open-power/hostboot) plugins
code for converting PEL sections to human readable format.

## Using the parser

### Console utility _esel_
Console utility allows to parse eSEL data and print it in several ways:
formatted text (similar to `errl` output), JSON or hex/bin dump.
The source data can be read from a file, OpenBMC event, or HBEL partition of
PNOR flash. Use `esel --help` for more details.

### Shared library _libeselparser.so_
The library provides a C++ API to convert raw eSEL data to object-oriented
instances. All classes, functions and variables of the library are placed
inside `eSEL` namespace.
In the event of the parser could not process the input data, the
`eSEL::InvalidFormat` exception will be thrown, the exception contains a short
description of the error.

Usage example:
```c++
#include <eselparser/event.hpp>
...
const uint8_t eselBlob[] = { ... };
try {
    eSEL::Event event;
    event.parse(eselBlob, sizeof(eselBlob));
    for (auto section : event.getSections()) {
        std::cout << "Section " << section->name() << std::endl;
        for (auto param : section->headerParams())
            std::cout << param.name() << ": " << param.value() << std::endl;
        for (auto param : section->payloadParams())
            std::cout << param.name() << ": " << param.value() << std::endl;
    }
}
catch(const eSEL::InvalidFormat& ex) {
    std::cerr << "Error parsing eSEL: " << ex.what() << std::endl;
}
```

## Project structure
Project consist of 4 modules:
1. _praser_: core of parser library;
2. _hbplugins_: static library used for embedding HostBoot's plugins into the
   parser;
3. _util_: user's console utility;
4. _test_: unit tests.

## Build
The following environment must be available to build the project:
- C++17 compiler;
- Perl with installed XML::Simple package;
- Autotools (automake and autoconf) with autoconf-archive.

Build sequence:
1. Remake the GNU Build System files:
   `./bootstrap.sh`
2. Update git submodules to get HostBoot's source code. This step is optional
   if you use external path, see `configure --help` for details;
3. Configure the project:
   `./configure`
4. Build the library:
   `make`

### Unit tests
The project contains unit test module to check the parser library functionality.
To build and run unit test module use appropriate target:
`make check`
The target system must have _gtest_ package installed.

### Valgrind
To check the library with Valgrind tools use appropriate target:
`make check-valgrind`
The target system must have _valgrind_ package installed.

## Known bugs
Some of HostBoot's plugins allocate memory and don't free it. The problem code
is in HostBoot's factory of ERRL parsers, but it happens only once at plugin
initialization routine and doesn't lead to persistent memory leak.

## Limitations
Plugin for parsing PRDF data is not supported yet.
