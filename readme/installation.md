# Installation

The project consits of two main parts:

- [Reflection library](../library)
- [Code generator](../generator)

The generator uses **Clang** libraries to analyse C++ source code. By default the libraries are linked  
statically to make distribution easier and provide precompiled binary [releases](https://github.com/chocolacula/easy_reflection_cpp/releases).  

First of all you should decide this option is suitable for you or you have to build the generator manually.  
If you chose manual option, be patient and update `llvm` along with other submodules first:

```bash
git submodule update --init --recursive
```
Then configure and build generator, CMake do everything for you.

> **Note:** If during **generation** you faced errors like `stddef.h` or `stdarg.h` not found, check include folders, perhaps you need a few symlinks. Don't ignore them, it would lead to analysis errors. 

### Windows

You can use MSVC to build your project but the generator works with `compile_commands.json` for analysis, so you should have it.  
Visual Studio CMake generators doesn't create that file, but you can use [Ninja](https://ninja-build.org/) instead.

In total, to build generator and use Easy Reflection on Windows you have to install and add to `PATH` variable:

- Git
- Python
- MSVC compiler
  - C++ ATL
  - Windows SDK
- CMake
- Ninja

### Docker

The repository also provides a `Dockerfile` which initializes `Ubuntu 22.04` environment, builds everything and runs tests on startup.

## Further steps

Another submodule is [vcpkg](https://github.com/microsoft/vcpkg) which manages most of the dependencies, all of them will be installed by CMake automatically.

> **Note:** You need **Python** to obtain project version from `vcpkg.json` manifest file. You probably already have it, please install it otherwise.

After you have installed all dependencies you should made a decision you wanna use [simdjson](https://github.com/simdjson/simdjson) for parsing or don't.  
Native for the solution parser will be available anyway but it's not so fast, though a bit more flexible in map parsing.  
If for some reason you wanna reduce number of dependencies you can exclude `simdjson` via CMake option:

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

You can install libraries from package manager of your distro such as **Ubuntu** or **Arch Linux** or build manually:

```bash
cd llvm-project/build

cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -DLLVM_ENABLE_RTTI=ON -DLLVM_LINK_LLVM_DYLIB=ON -DCLANG_LINK_CLANG_DYLIB=ON ../llvm
```

### Apple

Apple's macOS doesn't have standard Clang libs by default, but happily you can use brew.

```bash
brew install llvm
```

Don't forget to specify the path to LLVM.

```bash
-DLLVM_DIR=/opt/homebrew/opt/llvm/lib/cmake/llvm
```

### Windows

MSVC doesn't support it at all but there is some dark magic with MinGW and Clang you can dive in. Cannot help with that unfortunately.
