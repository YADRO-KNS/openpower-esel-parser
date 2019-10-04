#
# Build instructions for parser core library.
#

# Library to build
lib_LTLIBRARIES = libeselparser.la

# Header files to install
libeselparser_ladir = $(includedir)/eselparser
libeselparser_la_HEADERS = \
	event.hpp \
	fmtexcept.hpp \
	param.hpp \
	section.hpp \
	section_ph.hpp \
	section_ps.hpp \
	section_ud.hpp \
	section_uh.hpp \
	sel_record.hpp \
	setup.hpp

# Source files
libeselparser_la_SOURCES = \
	event.cpp \
	event.hpp \
	fmtexcept.hpp \
	hexdump.hpp \
	hexdump.cpp \
	ltables.hpp \
	ltables.cpp \
	param.hpp \
	param.cpp \
	params_col.hpp \
	params_col.cpp \
	section.cpp \
	section.hpp \
	section_ph.cpp \
	section_ph.hpp \
	section_ps.cpp \
	section_ps.hpp \
	section_ud.cpp \
	section_ud.hpp \
	section_uh.cpp \
	section_uh.hpp \
	sel_record.cpp \
	sel_record.hpp \
	setup.hpp

# Linking with hostboot's plugins static library
libeselparser_la_CXXFLAGS = -I$(top_srcdir)/hbplugins
libeselparser_la_LIBADD = $(top_builddir)/hbplugins/libhbplugins.la

# Library version information
libeselparser_la_LDFLAGS = -version-info 1:0:0
