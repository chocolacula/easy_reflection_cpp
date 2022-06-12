# Easy Reflection solution for C++

[![CMake build and test](https://github.com/chocolacula/reflection_cpp/actions/workflows/cmake.yml/badge.svg)](https://github.com/chocolacula/reflection_cpp/actions/workflows/cmake.yml)

It parses C++ sourcecode for special attributes. In the simplest situation you only need mark an object by `ER_REFLECT()` macro. All other work will be done by code generation tool and reflection library.

The main idea is use kinda dynamic typing and **Run Time Type Checking** for some type agnostic operations, like copying or taking name of a type.  
It makes possible determine a variable type and do right job - print, serialize/deserialize or whatever.

## Features

- Linux, MacOS and Windows support
- translate enums to string and vise versa
- support of stl containers like `std::vector`, `std::list`, `std::map`, etc.
- smart pointers support
- native serialization directly to an object and without third parties for:
  - **JSON**
  - **YAML 1.2** even with anchors, but keep in mind that variables behind anchors have to have the same type.
  - binary with **Variable Length Quantity** to reduce number of bytes
- debug printing
- understandable errors

## Quick start

Take a look at [Installation](readme/installation.md) guide and install the solution.

Then define your object and use `ER_REFLECT()` attribute:

```cpp
ER_REFLECT()
class Object {
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

For more details see [How To Use](readme/installation.md).

## Performance

The repository includes `benchmarks` folder, feel free to check it on your own hardware.

JSON on average **Core i5** laptop is faster then [nlohmann json](https://github.com/nlohmann/json).
Serialization is the same fast as [rapid json](https://github.com/Tencent/rapidjson), deserialization is little faster with `simdjson` parser and more then twice slower without.

YAML is blazingly faster then [yaml-cpp](https://github.com/jbeder/yaml-cpp), if I did the benchmark right.

> **Note:** Deserialization comparisson is not absolutely fair.  
Other libraries not always convert string represented values to `int`, `float` or `bool` and don't create instances of `std::string` until you call something like `.get<int>()`.  
Easy Reflection, on the other hand, provides ready-made object with all values within. And it's still fast despite it takes some time.

![Core i5 benchmarks](./benchmarks/performance_chart.png)

The length in bytes of serialized in different ways objects has the following relation:

![Memory](./benchmarks/memory_chart.png)

## TO DO

- Make `TypeId` compile time generated
- Cleanup all `TODO`s in the code
- Default value in case of `null` in parsers
- Prettify `reflection::print()`
- Add default constructable **SFINAE** guard
- Add support of move semantic
- Optimize size of `Expected<>` in parsers
- Test YAML parser extensively
- Test wide strings
- Add parent class fields to serialized fields
- Add support of user's template classes with different number of template arguments
- Add custom allocators in supported containers
