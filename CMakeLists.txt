cmake_minimum_required(VERSION 3.10)

execute_process(COMMAND python3 ${CMAKE_SOURCE_DIR}/version.py ${CMAKE_SOURCE_DIR}/vcpkg.json
    OUTPUT_VARIABLE VCPKG_MANIFEST_PROJECT_VERSION)

cmake_policy(SET CMP0048 NEW)
project(easy_reflection VERSION ${VCPKG_MANIFEST_PROJECT_VERSION})

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
