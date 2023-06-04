# Installation

The generator as a part of this project uses **Clang** libraries to analyse C++ source code.  
You can build Clang [from source](https://clang.llvm.org/get_started.html) or install from [vcpkg](https://github.com/microsoft/vcpkg).  

These libs link dynamically by default. There is an option of statically linking also.  
Set `-DLINK_CLANG_STATIC=ON` while building the generator in this case.

## Linux

In Linux, hopefully, you can use libs from repositories of your distro and save a bit of time.  
It tested with LLVM and Clang libraries from **Ubuntu** or **Arch Linux** repos.  

If you wanna build them from source, use required flags:

```bash
-DLLVM_ENABLE_PROJECTS=clang
-DLLVM_ENABLE_RTTI=ON
```

and for enabling dynamic linking

```bash
-DLLVM_LINK_LLVM_DYLIB=ON
-DCLANG_LINK_CLANG_DYLIB=ON
```

And of course, don't forget to provide `-DCMAKE_BUILD_TYPE=Release`.

> **Note:** If you faced errors like `stddef.h` or `stdarg.h` not found, check include folders, perhaps you need few symlinks.  
It's quite old problem and easy to google. Do not ignore them, it would lead to analysis errors e.g. missed template parents of an analyzed class.

### Docker

The repository also provides a `Dockerfile` which initializes `Ubuntu 22.04` environment,  
copy project files, builds it and runs tests on startup.

## Apple

Apple's macOS doesn't have precompiled Clang libs, so you have to build them from source, the rest is almost the same.  
See [Linux](#linux) for more information.

## Windows

Unfortunately it's not as easy as on POSIX systems, but I did most of work for you.  

On Windows you can avoid building the generator - just use latest binary release.  
There is nowhere to take precompiled **Clang** libraries, even distributed with [MSYS2](https://packages.msys2.org/package/mingw-w64-clang-x86_64-clang?repo=clang64) are not suitable.  
So, if you wanna build generator, you should build the libraries first.

You have to install and add to `PATH` variable:

- Git
- Python
- MSVC compiler
  - C++ ATL
  - Windows SDK
- CMake
- Ninja

Clone llvm-project from [GitHub](https://github.com/llvm/llvm-project) and create build directory as usual.  

Then open **Command Prompt for VS** as administrator and run commands:  

```cmd
llvm-project\build>
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -DLLVM_ENABLE_RTTI=ON -DLLVM_USE_CRT_RELEASE=MT -Thost=x64 ..\llvm
cmake --build . --config Release -j 8 -t install
```

> **Note:** `-DLLVM_LINK_LLVM_DYLIB=ON` is not available on Windows if you compile Clang by MSVC.  
But it's a valid parameter if you wanna do kinda recursive thing and compile Clang libs by Clang.  

Perhaps, you have to provide correct path to LLVM's CMake config files in `CMAKE_PREFIX_PATH`.

We have already build Clang and libs statically, don't forget to set few variables:

```cmd
LINK_CLANG_STATIC=ON
VCPKG_TARGET_TRIPLET="x64-windows-static"
```

### A piece of bad news for MSVC users

You can use MSVC to build your project and Easy Reflection but code generation tool `er_gen`  
uses Clang `LibTooling` which works with `compile_commands.json` for analysis, so you should have it.  

Visual Studio CMake generator doesn't create that file unfortunately, but you can use [Ninja](https://ninja-build.org/) instead.

## Further steps

The next step is update all submodules

```shell
git submodule update --init --recursive
```

One of the submodules is [vcpkg](https://github.com/microsoft/vcpkg) which manages most of the dependencies, all of them will be installed by CMake automatically.

> **Note:** The project version is obtaining by `python` from `vcpkg.json` manifest file.  
Python is dependency of everything now and most likely you already have it, otherwise please install it manually.

After you have installed all dependencies you should made a decision do you wanna use [simdjson](https://github.com/simdjson/simdjson) for parsing or don't.  
Native for the solution parser will be available anyway but it's slower, though a bit more flexible in map parsing.  
If for some reason you wanna reduce number of dependencies you can exclude `simdjson` via CMake option:

```bash
-DUSE_SIMD_JSON=OFF
```
