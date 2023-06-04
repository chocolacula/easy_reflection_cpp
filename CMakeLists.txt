cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_program(PYTHON "python3")
if(NOT PYTHON)
    find_program(PYTHON "python")
endif()

execute_process(COMMAND ${PYTHON} ${CMAKE_SOURCE_DIR}/version.py ${CMAKE_SOURCE_DIR}/vcpkg.json OUTPUT_VARIABLE VCPKG_MANIFEST_PROJECT_VERSION)

if(NOT VCPKG_MANIFEST_PROJECT_VERSION)
    message(WARNING "Cannot get project version")
endif()

cmake_policy(SET CMP0048 NEW)
project(easy_reflection VERSION ${VCPKG_MANIFEST_PROJECT_VERSION})

include(Dart)
enable_testing()

option(USE_SIMD_JSON "Enable SIMD JSON deserialization provider" ON)

if (USE_SIMD_JSON)
    add_definitions(-DUSE_SIMD_JSON)
endif()

add_subdirectory(library)
add_subdirectory(generator)
add_subdirectory(example)
add_subdirectory(tests)
add_subdirectory(benchmarks)
