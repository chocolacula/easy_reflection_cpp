# Installation

The project consits of two main parts:

- [Reflection library](../library)
- [Code generator](../generator)

The Easy Reflection C++ Generator utilizes **Clang** libraries to analyze C++ source code. To support reflection attributes in the source code, I had to patch `llvm` and link it as a dependency. Therefore, the only option is to build `llvm` from source. Be patient, as this process will not be quick(about 45m on a GitHub runner).

To analyze your project and generate reflection code, ensure you have a `compile_commands.json` file. If you're using CMake, you can easily enable this by adding the following line to your `CMakeLists.txt` file:

```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

## Windows

Visual Studio CMake generators do not create `compile_commands.json`. However, you can use [Ninja](https://ninja-build.org/) instead.

To build `er_gen` and use reflection on Windows, make sure to install the following dependencies and add them to your `PATH`:

- Git
- CMake
- Ninja

If you choose the MSVC compiler, additionally install:

- C++ ATL
- Windows SDK

## Further steps

Another submodule is [vcpkg](https://github.com/microsoft/vcpkg) which manages most of the dependencies, all of them will be installed by CMake automatically.

After installing all the dependencies, you need to decide whether you want to use `simdjson` for parsing or not. While `simdjson` provides the fastest performance on supported hardware, `rapidjson` might be faster in virtual environments. The solution includes a native parser that is available regardless. It offers more flexibility when it comes to map parsing but not as fast.

If, for some reason, you want to exclude `simdjson` from the dependencies, you can use the following **CMake** option:

```bash
-DUSE_SIMD_JSON=OFF
```

## Docker

This repository includes a `Dockerfile` that sets up an `Ubuntu 22.04` environment, builds the generator, and runs tests on startup.

## Troubleshooting

If you encounter errors like `stddef.h` or `stdarg.h` not being found during the generation process, add the following line to your `CMakeLists.txt` file:

```cmake
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
```
