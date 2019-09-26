AC_PREREQ([2.69])
AC_INIT([eselparser],
        [m4_esyscmd_s([git describe --always --long 2>/dev/null])],
        [https://github.com/YADRO-KNS/openpower-esel-parser/issues])
AC_LANG([C++])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign dist-xz])
AM_SILENT_RULES([yes])
AX_CHECK_ENABLE_DEBUG([no])

# Check for programs
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_PROG_SED
AC_PROG_GREP
AM_PROG_AR
AC_HEADER_STDC

# Check for perl (used by HostBoot)
AC_PATH_PROG([HB_PERL], [nativeperl])
AS_IF([test "x${HB_PERL}" = "x"], [AC_PATH_PROG([HB_PERL], [perl])])
AS_IF([test "x${HB_PERL}" = "x"], [AC_MSG_ERROR([perl not found])])
AC_MSG_CHECKING([for perl module XML::Simple])
AS_IF([${HB_PERL} -e "use XML::Simple; exit" > /dev/null 2>&1],
      [AC_MSG_RESULT([yes])],
      [AC_MSG_RESULT([no])
       AC_MSG_ERROR([XML::Simple package for Perl required but not found])])
AC_SUBST([HB_PERL])

# Library build configuration
LT_INIT

# Compiler flags
AX_CXX_COMPILE_STDCXX([17], [noext])
AX_APPEND_COMPILE_FLAGS([-fpic -Wall -Wextra], [CXXFLAGS])

# Configuration property: path to HostBoot source code
AC_ARG_VAR([HOSTBOOT_SRC_DIR], [Path to HostBoot source code directory])
AS_IF([test "x${HOSTBOOT_SRC_DIR}" = "x"], [HOSTBOOT_SRC_DIR="$(readlink -f ${srcdir}/hostboot)"])
AC_MSG_CHECKING([HostBoot source code in ${HOSTBOOT_SRC_DIR}])
AS_IF([test -f "${HOSTBOOT_SRC_DIR}/src/HBconfig"],
      [AC_MSG_RESULT([yes])],
      [AC_MSG_RESULT([no])
       AC_MSG_ERROR([invalid path to HostBoot source code directory. Please, set HOSTBOOT_SRC_DIR variable or update submodules.])])
AC_SUBST([HOSTBOOT_SRC_DIR])

# Define HostBoot's version
AC_MSG_CHECKING([for HostBoot revision])
AS_VAR_SET([HOSTBOOT_REVISION], [$(git -C ${HOSTBOOT_SRC_DIR} describe --always --long 2>/dev/null)])
AS_IF([test "x${HOSTBOOT_REVISION}" = "x"], [HOSTBOOT_REVISION="unknown"])
AC_MSG_RESULT([${HOSTBOOT_REVISION}])
AC_DEFINE_UNQUOTED([HOSTBOOT_REVISION], ["${HOSTBOOT_REVISION}"])

# Configuration properties: default paths
AC_ARG_VAR([DEFAULT_HB_SYMBOLS], [Default path to HostBoot symbol file])
AS_IF([test "x${DEFAULT_HB_SYMBOLS}" = "x"], [DEFAULT_HB_SYMBOLS="hbicore.syms"])
AC_DEFINE_UNQUOTED([DEFAULT_HB_SYMBOLS], ["${DEFAULT_HB_SYMBOLS}"])
AC_ARG_VAR([DEFAULT_HB_STRINGS], [Default path to HostBoot string file])
AS_IF([test "x${DEFAULT_HB_STRINGS}" = "x"], [DEFAULT_HB_STRINGS="hbotStringFile"])
AC_DEFINE_UNQUOTED([DEFAULT_HB_STRINGS], ["${DEFAULT_HB_STRINGS}"])
AC_ARG_VAR([DEFAULT_OCC_STRINGS], [Default path to OCC string file])
AS_IF([test "x${DEFAULT_OCC_STRINGS}" = "x"], [DEFAULT_OCC_STRINGS="occStringFile"])
AC_DEFINE_UNQUOTED([DEFAULT_OCC_STRINGS], ["${DEFAULT_OCC_STRINGS}"])
AC_ARG_VAR([DEFAULT_FSP_TRACE], [Default path to FSP trace utility])
AS_IF([test "x${DEFAULT_FSP_TRACE}" = "x"], [DEFAULT_FSP_TRACE="fsp-trace"])
AC_DEFINE_UNQUOTED([DEFAULT_FSP_TRACE], ["${DEFAULT_FSP_TRACE}"])

# Valgrind
AX_VALGRIND_DFLT([memcheck], [on])
AX_VALGRIND_CHECK
AM_EXTRA_RECURSIVE_TARGETS([check-valgrind])
m4_foreach([vgtool], [valgrind_tool_list],
  [AM_EXTRA_RECURSIVE_TARGETS([check-valgrind-]vgtool)])

# Check for gtest specific functions
PKG_CHECK_MODULES([GTEST], [gtest_main], [], [AC_MSG_NOTICE([unit tests disabled: gtest not found])])
AX_PTHREAD([GTEST_CFLAGS+=" -DGTEST_HAS_PTHREAD=1 "],[GTEST_CFLAGS+=" -DGTEST_HAS_PTHREAD=0 "])

AC_CONFIG_FILES([Makefile
                 hbplugins/Makefile
                 parser/Makefile
                 util/Makefile
                 test/Makefile])
AC_OUTPUT
