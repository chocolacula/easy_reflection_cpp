cmake_minimum_required(VERSION 3.19)

include(cmake/tools.cmake)
update_submodule(vcpkg)
set_policies()

set(CMAKE_TOOLCHAIN_FILE vcpkg/scripts/buildsystems/vcpkg.cmake)

# Get version from vcpkg.json manifest
file(READ "${CMAKE_SOURCE_DIR}/vcpkg.json" VCPKG_MANIFEST_JSON)

string(JSON
    VCPKG_MANIFEST_PROJECT_VERSION
    ERROR_VARIABLE VCPKG_VERSION_ERROR
    GET ${VCPKG_MANIFEST_JSON} "version")

if(VCPKG_VERSION_ERROR)
    message(WARNING ${VCPKG_VERSION_ERROR})
    set(VCPKG_MANIFEST_PROJECT_VERSION "0.0.0")
endif()

# Enable vcpkg features
option(ENABLE_TESTS      "Enable tests"      ON)
option(ENABLE_GENERATOR  "Enable generator"  OFF)
option(ENABLE_BENCHMARKS "Enable benchmarks" OFF)

if(ENABLE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()
if(ENABLE_GENERATOR)
    list(APPEND VCPKG_MANIFEST_FEATURES "generator")
endif()
if(ENABLE_BENCHMARKS)
    list(APPEND VCPKG_MANIFEST_FEATURES "benchmarks")
endif()

# Setup project
project(easy_reflection VERSION ${VCPKG_MANIFEST_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

include(Dart)
enable_testing()

add_subdirectory(library)
add_subdirectory(example)

if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()
if(ENABLE_GENERATOR)
    add_subdirectory(generator)
endif()
if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
