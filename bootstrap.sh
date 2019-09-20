#!/bin/sh -e

AUTOCONF_FILES="Makefile.in aclocal.m4 ar-lib autom4te.cache compile \
                config.guess config.log config.h.in config.sub config.status \
                configure depcomp install-sh ltmain.sh missing \
                *libtool test-driver .deps"

if [ "$1" = "clean" ]; then
   [ ! -f Makefile ] || make maintainer-clean
   for FILE in ${AUTOCONF_FILES}; do
     find -name "${FILE}" | xargs -r rm -rf
   done
   exit 0
 fi

autoreconf -i
echo 'Run "./configure ${CONFIGURE_FLAGS} && make"'
