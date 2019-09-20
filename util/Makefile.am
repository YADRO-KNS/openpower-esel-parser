#
# Build instructions for console utility.
#

bin_PROGRAMS = esel

# Source files
esel_SOURCES = \
	main.cpp \
	printer.hpp \
	printer.cpp \
	task.hpp \
	task.cpp

# Build flags
esel_CXXFLAGS = \
	-Wl,--no-undefined \
	-I$(top_srcdir)/parser

# Linker flags, using std::filesystem depends on fs library for pre-GCC 9 compilers
esel_LDFLAGS = -lstdc++fs

# Linking with parser library
PARSER_LIB = $(top_builddir)/parser/libeselparser.la
esel_DEPENDENCIES = $(PARSER_LIB)
esel_LDADD = $(PARSER_LIB)
