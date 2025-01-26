cmake_minimum_required(VERSION 3.19)

include(cmake/tools.cmake)
update_submodule(vcpkg)
set_policies()

set(CMAKE_TOOLCHAIN_FILE vcpkg/scripts/buildsystems/vcpkg.cmake)

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
project(easy_reflection VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

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
