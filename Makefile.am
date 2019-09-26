# Additional target to format source code
format:
	clang-format -style=file --verbose -i \
		$(top_srcdir)/hbplugins/*.H \
		$(top_srcdir)/hbplugins/*.hpp \
		$(top_srcdir)/hbplugins/*.cpp \
		$(top_srcdir)/parser/*.hpp \
		$(top_srcdir)/parser/*.cpp \
		$(top_srcdir)/util/*.hpp \
		$(top_srcdir)/util/*.cpp \
		$(top_srcdir)/test/*.cpp

SUBDIRS = hbplugins parser util test
