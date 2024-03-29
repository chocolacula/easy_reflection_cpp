# How To Use

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

To get data from **MongoDB** and map it to the class instance you only need to do few simple actions:

Add `[[er::reflect(...)]]` attribute before class name

```cpp
class [[er::reflect("data")]] Employee {
 public:
  std::string name;
...
```
You have the ability to customize which part of an object will be reflected by utilizing the following options:

- `"base"` - include fields and methods from base class
- `"nonPublic"` - include protected and private
- `"data"` - fields
- `"func"` - methods and static functions
- `"all"` - combination of all options above

The empty options is equal to `"data"`.

Standard serializers skip non public and static fields, to change that, you have to write you own serializer. Just copy existing one to somewhere under `er::serialization` namespace and change the line:

```cpp
for (auto&& record : o.get_fields()) {
```

to something like:

```cpp
for (auto&& record : o.get_fields(Access::kPublic | Access::kProtected | Access::kPrivate)) {
```

Please be aware of using `static` fields. In the event that they are modified during the serialization/deserialization process, it may lead to unexpected behavior for other instances of the class. It is recommended to avoid writing to `static` fields in such scenarios. However, if it is absolutely necessary to access them, you can do so by passing a `nullptr` of the specific type.

```cpp
Employee* ptr = nullptr;
auto reflected = reflection::reflect(ptr);
```

You can use aliases if the data in json have different field names, for example you wanna rename field `name` to `full_name` during serialization:

```cpp
class [[er::reflect]] Employee {
 public:
  [[er::alias("full_name")]]
  std::string name;
...
```

To exclude any undesired fields use `[[er::ignore]]` attribute.

Then setup the generator tool by editing `config.yaml` file. The most important key in the file is:

```yaml
input:
  - /path/to/analyzing/file1.h
  - /path/to/analyzing/file2.h
  - /path/to/analyzing/directory
```

Any path could leeds to a directory or a file. The kind of path is determined by filesystem. All files in specified directory and its subdirectories will be analyzed automatically.

> **Note:** Do not specify project's root directory it leads to run AST traversal for each file which is long and definitely not what you want.

Specify output directory for generated files:

```yaml
output_dir: project_path/generated
```

Run the generator binary `er_gen` like:

```shell
er_gen -c <path to config.yaml>
```

For each object will be generated one `.h` and one `.cpp` file. Eventually they will be included in `reflection.h` and `reflection.cpp`.  

You should add the source code file to the project's sources. With this fact **CMake** and `add_custom_command()` slightly simplifies the generation.  

```cmake
add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/generated/reflection.cpp
    COMMAND er_gen -c ${CMAKE_CURRENT_SOURCE_DIR}/config.yaml
    DEPENDS
        path/to/file/with/types_for_reflect.h
    COMMENT "Generating reflection code")
```

As you can see `reflection.cpp` is in `OUTPUT` parameter and `types_for_reflect.h` is in `DEPENDS`. It means generation will start just before building if `OUTPUT` doesn't exist or `DEPENDS` has changed.

Generated headers have relative path to origin file with source code. It doesn't give you a 100% guarantee of project structure consistency, but still makes possible to use generated files on multiple machines with different storage configuration. And it's generally fine to commit them to a repository.

And of course link the reflection library

```cmake
target_link_libraries(${PROJECT_NAME} PRIVATE reflection)
```

Then you only need include the header and have fun with desired reflection in C++. The last step is use deserialization from json:

```cpp
#include "generated/reflection.h"
#include "er/serialization/simd_json.h"

...
auto employee = serialization::simd_json::from_string<Employee>(str_from_mongo).unwrap();
...
```

Please see [example](../example) for more details.

## Strings

It's not easy to make decision how to deserialize strings. `std::string_view` and `const char*` are strings but in the same time just references to data nomater const or not.

While you deserialize data which you received from someware you cannot just take a reference, you need something to own data. Use `std::string` to own and serialize/deserialize data and reference types like something const which you could print or analyze but not set while deserialization. For this reason reflection marks `std::string_view` and `C strings` by special `read only` attribute and does not serialize.
