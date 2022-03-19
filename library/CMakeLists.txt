cmake_minimum_required(VERSION 3.10)

project(reflection VERSION ${CMAKE_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
include_directories(RE-flex/include)

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

if(WIN32)
    set(RE_FLEX_LIB ${CMAKE_CURRENT_SOURCE_DIR}/RE-flex/vs/reflex.lib)
else()
    set(RE_FLEX_LIB ${CMAKE_CURRENT_SOURCE_DIR}/RE-flex/lib/libreflex.a)
endif()

if(NOT EXISTS ${RE_FLEX_LIB})
    message("make RE-flex library")
    if(WIN32)
        execute_process(
            COMMAND RE-flex_build.bat
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/library)
    else()
        execute_process(
            COMMAND ./build.sh
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/library/RE-flex)
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${RE_FLEX_LIB})

if (USE_SIMD_JSON)
    find_package(simdjson CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC simdjson::simdjson)
endif()
