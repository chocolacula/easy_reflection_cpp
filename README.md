# Easy Reflection solution for C++

It parses C++ sourcecode for special attributes. In the simplest situation you only need mark an object by ```ER_REFLECT()``` macro. All other work will be done by code generation tool and reflection library.

The main idea is use kinda dynamic typing and <b>Run Time Type Checking</b> for some type agnostic operations, like copying or taking name of a type.<br>
It makes possible determine a variable type and do right job - print, serialize/deserialize or whatever.

## Features

- translate enums to string and vise versa
- support stl containers like ```std::vector```, ```std::list```, ```std::map```, etc.
- native serialization directly to an object and without third parties for:
  - <b>JSON</b>
  - <b>YAML 1.2</b> even with anchors, but keep in mind that variables behind anchors have to have the same type.
  - binary with <b>Variable Length Quantity</b> to reduce number of bytes
- debug printing

## Installation

At first you have to download and compile <b>Clang</b>. The generator as a part of this project uses Clang to analyse C++ source code. You can build Clang [from source](https://clang.llvm.org/docs/LibASTMatchersTutorial.html), install from [vcpkg](https://github.com/microsoft/vcpkg) or from repositories of your distro. Please pay attention to ```generator/CMakeLists.txt```, it's fit for LLVM and Clang libraries from <b>Ubuntu</b> or <b>Arch Linux</b> repos or built from source with options:
```
-DLLVM_LINK_LLVM_DYLIB=ON
-DCLANG_LINK_CLANG_DYLIB=ON
-DLLVM_ENABLE_RTTI=ON
```
change the file if needed.

> <b>Note:</b> Compile Clang from sources takes <b>A LOT</b> of time, literally hours with freezes and fails on average laptop, think about using precompiled libs from your distro.

> <b>Note:</b> If you faced errors like ```stddef.h``` or ```stdarg.h``` not found, check include folders, perhaps you need few symlinks. It's quite old problem and easy to google. Do not ignore them, it would lead to analysis errors e.g. missed template parents of an analyzed class.

The next step is update all submodules<br>

```shell
git submodule update --init --recursive
```

### Docker

The repository also provides a ```Dockerfile``` which initializes ```Ubuntu 20.04``` environment, clones the project, builds it and runs tests on startup.

## How to use

Lets assume you are developer of a company employees registry and have a class:
```cpp
class Employee {
 public:
  std::string name;
  std::string position;
  uint16_t room;
  std::vector<uint32_t> project_ids;

 private:
  uint8_t age;
  uint8_t total_experience;
}
```

To get data from <b>MongoDB</b> and map it to the class instance you only need to do few simple actions:<br>

Include header with defined attributes and add ```ER_REFLECT()``` attribute before class declaration

```cpp
#include "er/attributes.h"

ER_REFLECT()
class Employee {
 public:
  std::string name;
...
```

in this case <b>private</b> fields will be omitted, to reflect them add:

```cpp
#include "er/attributes.h"

ER_REFLECT(WithNonPublic)
class Employee {
 public:
  std::string name;
...
```

But note they are fields for <b>reflection</b> NOT for <b>serialization</b>. It's possible to use values or print them. Standard serializers skip non public and static fields, to change that, you have to write you own serializer. Just copy one to somewhere under ```er::serialization``` namespace and change line:

```cpp
for (auto&& record : o.get_fields()) {
```

to something like:

```cpp
for (auto&& record : o.get_fields(Access::kPublic | Access::kProtected | Access::kPrivate)) {
```

Please be aware of using ```static``` fields. It's possible to have access to them by passing ```nullptr``` of particular type:

```cpp
Employee* ptr = nullptr;
auto reflected = reflection::reflect(ptr);
```

If The fields could be set while serialization/deserialization process, it would be at least unexpected for other instances of a class. To exclude any non desired fields use ```ER_EXCLUDE``` attribute.

Then setup the generator tool by editing ```config.yaml``` file. The most important key in the file is:

```yaml
input:
  - /path/to/analyzing/file1.h
  - /path/to/analyzing/file2.h
  - /path/to/analyzing/directory
```

Any path could leeds to a directory or a file. The kind of path is determined by filesystem. All files in specified directory and its subdirectories will be analyzed automatically.

> <b>Note:</b> Do not specify project's root directory it leads to whole project analysis which is long and definitely not what you want.

Run generator like:
```shell
er_gen -c <path to config.yaml>
```

All generated headers will be included to one big ```reflection.h``` header. You only need include it and have fun with desired reflection in C++. With this fact <b>CMake</b> makes generation a little bit simpler with ```add_custom_command()``` set ```reflection.h``` in ```OUTPUT``` parameter and all files will be generated just before building the project.

Generated headers have relative path to origin file with source code. It doesn't give you a 100% guarantee of your project structure consistency, but still makes possible to use generated files on multiple machines with different storage configuration. And it's generally fine to commit them to a repository.

The last step is use deserialization from json:

```cpp
#include "er/serialization/json.h"

...
auto employee = serialization::json::from_string<Employee>(str_from_mongo).unwrap();
...
```

Please see ```example/main.cpp``` for more details.