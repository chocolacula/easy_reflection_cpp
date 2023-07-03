cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

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

cmake_policy(SET CMP0048 NEW)
project(easy_reflection VERSION ${VCPKG_MANIFEST_PROJECT_VERSION})

include(cmake/generate_reflection_code.cmake)
include(cmake/print_variables.cmake)

include(Dart)
enable_testing()

add_subdirectory(library)
add_subdirectory(generator)
add_subdirectory(example)
add_subdirectory(tests)
add_subdirectory(benchmarks)
