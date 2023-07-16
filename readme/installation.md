# Installation

The project consits of two main parts:

- [Reflection library](../library)
- [Code generator](../generator)

The Easy Reflection C++ Generator utilizes **Clang** libraries to analyze C++ source code. By default, these libraries are statically linked to facilitate distribution and provide precompiled binary [releases](https://github.com/chocolacula/easy_reflection_cpp/releases).

Before getting started, you should decide whether to use the precompiled binary release or build the generator manually. If you choose the manual option, configure and build the generator. CMake will handle everything for you.

Once you have the `er_gen` binary, you can start analyzing your project and generating code. To do this, ensure that your project produces a `compile_commands.json` file. If you're using CMake, you can easily enable this by adding the following line to your CMakeLists.txt file:

```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

### Troubleshooting

If you encounter errors like `stddef.h` or `stdarg.h` not being found during the generation process, add the following line to your `CMakeLists.txt` file:

```cmake
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
```

### Windows

Visual Studio CMake generators do not create `compile_commands.json`. However, you can use [Ninja](https://ninja-build.org/) instead.

To build `er_gen` and use reflection on Windows, make sure to install the following dependencies and add them to your `PATH`:

- Git
- Python
- CMake
- Ninja

If you choose the MSVC compiler, additionally install:

- C++ ATL
- Windows SDK

### Docker

This repository includes a `Dockerfile` that sets up an `Ubuntu 22.04` environment, builds the generator, and runs tests on startup.

## Further steps

Another submodule is [vcpkg](https://github.com/microsoft/vcpkg) which manages most of the dependencies, all of them will be installed by CMake automatically.

> **Note:** You need **Python** to obtain project version from `vcpkg.json` manifest file. You probably already have it, please install it otherwise.

After you have installed all the dependencies, you need to decide whether you want to use [simdjson](https://github.com/simdjson/simdjson) for parsing or not. The solution includes a native parser that is available regardless, but it's not as fast. However, the native parser is more flexible when it comes to map parsing. If, for some reason, you want to reduce the number of dependencies, you can exclude `simdjson` by using the following CMake option:

```bash
-DUSE_SIMD_JSON=OFF
```

## LLVM & Clang dynamic linking

It is not supported officialy but nevertheless it exists.

Replace lines in the end of [CMakeLists.txt](../generator/CMakeLists.txt)

```cmake
find_package(LLVM REQUIRED)

include_directories(${LLVM_INCLUDE_DIR})
link_directories(${LLVM_LIBRARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE LLVM clang-cpp)
```

### Linux

You can install libraries from package manager of your distro such as **Ubuntu** or **Arch Linux** or build them from source:

```bash
cd llvm-project/build

cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -DLLVM_ENABLE_RTTI=ON -DLLVM_LINK_LLVM_DYLIB=ON -DCLANG_LINK_CLANG_DYLIB=ON ../llvm
```

### Apple

Apple's macOS doesn't have standard Clang libs by default, but happily you can use `brew`.

```bash
brew install llvm
```

Don't forget to specify the path to LLVM.

```bash
-DLLVM_DIR=/opt/homebrew/opt/llvm/lib/cmake/llvm
```

### Windows

Regrettably, MSVC does not support for this feature. However, there is some dark magic with MinGW and Clang you can delve into. Unfortunately, I cannot provide assistance with that particular matter.
