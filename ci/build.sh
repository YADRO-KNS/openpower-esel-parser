#!/bin/sh -eu

#
# Build and test the project.
#

DOCKER_IMAGE="eselparser"
ROOT_DIR="$(cd $(dirname $0)/.. && pwd)"

# Build docker image
docker build --tag ${DOCKER_IMAGE} ${ROOT_DIR}/ci

# Get default HostBoot
[ -f ${ROOT_DIR}/hostboot/makefile ] || git submodule update --init

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
           make check-valgrind && \
           export LD_LIBRARY_PATH=${ROOT_DIR}/parser/.libs && \
           for I in ${ROOT_DIR}/test/data/*.bin; do \
             valgrind --error-exitcode=1 \
               ${ROOT_DIR}/util/.libs/esel --file \${I} >/dev/null || exit 1; \
           done"
