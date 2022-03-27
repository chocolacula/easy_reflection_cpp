FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y git g++ make cmake pkg-config \
  build-essential tar curl zip unzip \
  llvm-10-dev libclang-10-dev libclang-cpp10-dev

COPY . ./reflection_cpp
# or clone the repository directly from github
# RUN git clone https://github.com/chocolacula/reflection_cpp.git

RUN cd reflection_cpp \
  && git submodule update --init --recursive \
  && mkdir build && cd build \
  && cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  && cmake --build .

WORKDIR /reflection_cpp/build
CMD ctest