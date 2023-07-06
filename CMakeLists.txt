cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Get version from vcpkg.json manifest
find_program(PYTHON "python3")
if(NOT PYTHON)
    find_program(PYTHON "python")
endif()

if(NOT PYTHON)
    message(WARNING "Python is not found")
endif()

execute_process(COMMAND ${PYTHON} ${CMAKE_SOURCE_DIR}/version.py
    get -f ${CMAKE_SOURCE_DIR}/vcpkg.json
    OUTPUT_VARIABLE VCPKG_MANIFEST_PROJECT_VERSION
    ERROR_VARIABLE  VCPKG_VERSION_ERROR)

if(VCPKG_VERSION_ERROR)
    message(WARNING ${VCPKG_VERSION_ERROR})
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
cmake_policy(SET CMP0048 NEW)
project(easy_reflection VERSION ${VCPKG_MANIFEST_PROJECT_VERSION})

include(cmake/generate_reflection_code.cmake)
include(cmake/print_variables.cmake)

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
