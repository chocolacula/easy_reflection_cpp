FROM ubuntu:22.04

# Note! vcpkg requires for ARM builds:
#   - set env VCPKG_FORCE_SYSTEM_BINARIES=1
#   - installed Ninja build system
# it's also a valid setup for non ARM build

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y git g++ make ninja-build cmake pkg-config \
  build-essential tar curl zip unzip

COPY . ./reflection_cpp
# or clone the repository directly from github
# RUN git clone https://github.com/chocolacula/easy_reflection_cpp.git

ENV VCPKG_FORCE_SYSTEM_BINARIES=1

# build with GCC by default
RUN cd reflection_cpp \
  && mkdir build && cd build \
  && cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  && cmake --build .

WORKDIR /reflection_cpp/build
CMD ctest
