cmake_minimum_required(VERSION 3.10)

project(reflection VERSION ${CMAKE_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

set(SOURCES
        src/reflection/reflection.cpp
        src/err_helper.cpp
        src/variable/var.cpp
        src/variable/box.cpp
        src/tools/stringify.cpp
        src/serialization/json/json.cpp
        src/serialization/json/simd_json.cpp
        src/serialization/json/lexer_json.yy.cpp
        src/serialization/json/parser_json.cpp
        src/serialization/yaml/yaml.cpp
        src/serialization/yaml/lexer_yaml.yy.cpp
        src/serialization/yaml/parser_yaml.cpp
        src/serialization/binary/binary.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

update_submodule(RE-flex)

add_subdirectory(RE-flex)
target_link_libraries(${PROJECT_NAME} PRIVATE ReflexLibStatic)

option(USE_SIMD_JSON "Enable SIMD JSON deserialization provider" ON)

if (USE_SIMD_JSON)
    target_compile_definitions(${PROJECT_NAME} PUBLIC USE_SIMD_JSON)

    find_package(simdjson CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE simdjson::simdjson)
endif()
