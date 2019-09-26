#!/bin/bash -eu

#
# Local build.
#

ROOT_DIR="$(realpath $(dirname ${BASH_SOURCE[0]})/..)"
DOCKER_IMAGE="eselparser"

# Build docker image
docker build --tag ${DOCKER_IMAGE} ${ROOT_DIR}/ci

# Get default HostBoot
[[ -f ${ROOT_DIR}/hostboot/makefile ]] || { git submodule init && git submodule update; }

# Build the project and run tests
docker run \
  --interactive --tty --rm \
  --volume ${ROOT_DIR}:${ROOT_DIR}:Z \
  --workdir "${ROOT_DIR}" \
  --user $(id -u):$(id -g) \
  ${DOCKER_IMAGE} \
  bash -c "./bootstrap.sh && \
           ./configure && \
           make -j$(grep -c ^processor /proc/cpuinfo) && \
           make check-valgrind"
# TODO:
#           export LD_LIBRARY_PATH=${ROOT_DIR}/parser/.libs && \
#           for I in ${ROOT_DIR}/test/data/*.bin; do \
#             valgrind --tool=memcheck --error-exitcode=1 --gen-suppressions=all \
#               ${ROOT_DIR}/util/.libs/esel --file \${I} >/dev/null; \
#           done"
