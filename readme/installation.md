# Installation

The project consits of two main parts:

- Reflection library
- Code generator

The generator uses **Clang** libraries to analyse C++ source code. By default the libraries are linked  
statically to make distribution easier and provide precompiled binary [releases](https://github.com/chocolacula/easy_reflection_cpp/releases).  

First of all you should decide is this option suitable for you or you need build the generator manually.  
If you chose manual option, don't rush and update `llvm` with other submodules first:

```bash
git submodule update --init --recursive
```
Then configure and build generator, CMake build and link everything for you.

> **Note:** If you faced errors like `stddef.h` or `stdarg.h` not found, check include folders, perhaps you need few symlinks.  
It's quite old problem and easy to google. Do not ignore them, it would lead to analysis errors e.g. missed template parents of an analyzed class.

### Docker

The repository also provides a `Dockerfile` which initializes `Ubuntu 22.04` environment,  
builds everything and runs tests on startup.

## Windows

Unfortunately it's not as easy as on POSIX systems, but I did most of work for you.  
You have to install and add to `PATH` variable:

- Git
- Python
- MSVC compiler
  - C++ ATL
  - Windows SDK
- CMake
- Ninja

Then open **Command Prompt for VS** as administrator and run commands:  

```cmd
llvm-project\build>

cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -DLLVM_ENABLE_RTTI=ON -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_USE_CRT_RELEASE=MT -Thost=x64 ..\llvm

cmake --build . -j 8 -t install
```

### A piece of bad news for MSVC users

You can use MSVC to build your project and Easy Reflection but code generation tool `er_gen`  
uses Clang `LibTooling` which works with `compile_commands.json` for analysis, so you should have it.  

Visual Studio CMake generator doesn't create that file unfortunately, but you can use [Ninja](https://ninja-build.org/) instead.

## Further steps

Another submodule is [vcpkg](https://github.com/microsoft/vcpkg) which manages most of the dependencies, all of them will be installed by CMake automatically.

> **Note:** The project version is obtaining by `python` from `vcpkg.json` manifest file.  
Python is a dependency of everything these days and most likely you already have it, otherwise please install it manually.

After you have installed all dependencies you should made a decision do you wanna use [simdjson](https://github.com/simdjson/simdjson) for parsing or don't.  
Native for the solution parser will be available anyway but it's slower, though a bit more flexible in map parsing.  
If for some reason you wanna reduce number of dependencies you can exclude `simdjson` via CMake option:

```bash
-DUSE_SIMD_JSON=OFF
```

## LLVM & Clang dynamic linking

It is not official supported way and MSVS on Windows doesn't support it at all but nevertheless it exists. 

Replace `add_clang_executable` and `clang_target_link_libraries` to:

```cmake
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE LLVM clang-cpp)
```

### Linux

You can install libraries from package manager of your distro such as **Ubuntu** and **Arch Linux** or build manually:

```bash
cd llvm-project/build

cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -DLLVM_ENABLE_RTTI=ON -DLLVM_LINK_LLVM_DYLIB=ON -DCLANG_LINK_CLANG_DYLIB=ON ../llvm
```

### Apple

Apple's macOS doesn't have precompiled Clang libs, but happily you can use brew.

```bash
brew install llvm
```

Don't forget to specify the path to LLVM.

```bash
-DLLVM_DIR=/opt/homebrew/opt/llvm/lib/cmake/llvm
```
