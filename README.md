# Easy Reflection solution for C++

[![CMake build and test](https://github.com/chocolacula/reflection_cpp/actions/workflows/cmake.yml/badge.svg)](https://github.com/chocolacula/reflection_cpp/actions/workflows/cmake.yml)
<a href="https://github.com/fffaraz/awesome-cpp#reflection"><img src="https://cdn.rawgit.com/sindresorhus/awesome/d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg" alt="Listed on Awesome C++"></img></a>

It parses C++ source code for special attributes. In the most straightforward situation, you only need to mark an object by `[[er::reflect]]` attribute. All other work will be done by the code generation tool and reflection library.

The main idea is to use kinda dynamic typing for some type agnostic operations, like copying or getting the name of a type.  
It makes it possible to determine a variable type and do the right job - print, serialize/deserialize.

It's generally a proof of concept, created with the idea that it could be used for many years. If you are curious about the details of how it works, you can find them in [DEV article](https://dev.to/chocolacula/how-to-write-reflection-for-c-4527).  

## Features

- Linux, MacOS and Windows, x86 and ARM support
- translate enums to string and vice versa
- invoke methods
- support of stl containers like `std::vector`, `std::list`, `std::map`, etc.
- smart pointers support
- native serialization directly to an object and without third parties for:
  - **JSON**
  - **YAML 1.2** even with anchors, but keep in mind that variables behind anchors have to be the same type.
  - binary with **Variable Length Quantity** to reduce number of bytes
- debug printing
- understandable errors

## Quick start

Look at [Installation](readme/installation.md) guide and install the solution.

Then define your object and use `[[er::reflect]]`:

```cpp
class [[er::reflect]] Object {
 public:
  std::string field_str;
  int field_int;

  std::vector<int> field_vector;
}
```

And serialize/deserialize it in one shot:

```cpp
#include "generated/reflection.h"
#include "er/serialization/json.h"

using namespace serialization;

...
auto str = json::to_string<Object>(obj).unwrap();
obj = json::from_string<Object>(str).unwrap();
...
```

For more details see [How To Use](readme/how_to_use.md).

## Performance

The repository includes `benchmarks` folder, feel free to check it on your own hardware.

JSON is faster than [nlohmann json](https://github.com/nlohmann/json). Serialization is the same fast as [rapid json](https://github.com/Tencent/rapidjson), deserialization is a little faster with `simdjson` parser and more than twice slower without.

YAML is blazingly faster than [yaml-cpp](https://github.com/jbeder/yaml-cpp) if I did the benchmark right.

> **Note:** Other libraries do not always convert string-represented values to `int`, `float`, or `bool` and don't create instances of `std::string` until you call something like `.get<int>()`. **Easy Reflection**, on the other hand, provides ready-made objects with all values within.

<p align="center">
  <img src="./benchmarks/performance_chart.png" alt="Core i5 benchmarks">
</p>

<p align="center">
  <img src="./benchmarks/memory_chart.png" alt="Memory">
</p>
<p align="center">The ratio of the content length in bytes</p>

## Thanks

JetBrains for Open Source Support

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<a href="https://jb.gg/OpenSourceSupport">
  <img src="https://resources.jetbrains.com/storage/products/company/brand/logos/jb_beam.svg" alt="JetBrains Logo" style="width:128px;height:128px;">
</a>
