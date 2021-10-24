FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y g++ make cmake git llvm-dev libclang-10-dev && \
  ln -s /usr/lib/llvm-10/lib/libclang-cpp.so.10 /usr/lib/llvm-10/lib/libclang-cpp.so

RUN git clone https://github.com/chocolacula/reflection_cpp.git && cd reflection_cpp && \
  git submodule update --init --recursive && mkdir build && cd build && \
  cmake .. && make

WORKDIR /reflection_cpp/build
CMD ctest