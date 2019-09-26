#
# Build instructions for unit tests.
#

@VALGRIND_CHECK_RULES@

check_PROGRAMS = eselparser_test
TESTS = $(check_PROGRAMS)

# Source files
eselparser_test_SOURCES = \
	fmtexcept_test.cpp \
	parser_test.cpp

# Build flags
eselparser_test_CXXFLAGS = \
	-I$(top_srcdir)/parser \
	$(GTEST_CFLAGS)

# Libraries to link with
eselparser_test_LDADD = \
	$(GTEST_LIBS) \
	$(PTHREAD_LIBS)

# Linking with parser library
PARSER_LIB = $(top_builddir)/parser/libeselparser.la
eselparser_test_DEPENDENCIES = $(PARSER_LIB)
eselparser_test_LDADD += $(PARSER_LIB)

# Clean log files from valgrind
clean-local: clean-local-logs
.PHONY: clean-local-logs
clean-local-logs:
	-rm -f *.log
