FROM ubuntu:20.04

LABEL Description="eSEL parser builder"

# Install base system
ENV DEBIAN_FRONTEND noninteractive
RUN apt update
RUN apt upgrade --yes
RUN apt install --no-install-recommends --yes \
  autoconf \
  autoconf-archive \
  automake \
  g++ \
  git \
  libtool \
  libxml-parser-perl \
  libxml-simple-perl \
  make \
  pkg-config \
  valgrind

# Install Google test lib
RUN apt install --no-install-recommends --yes cmake
RUN mkdir /tmp/gtest && git clone --depth 1 --branch v1.8.x https://github.com/google/googletest.git /tmp/gtest
RUN cd /tmp/gtest && \
    cmake -DINSTALL_GTEST=ON . && \
    cmake --build . --target install
RUN apt purge --auto-remove --yes cmake
RUN rm -rf /tmp/gtest
