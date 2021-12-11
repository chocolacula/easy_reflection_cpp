cmake_minimum_required(VERSION 3.10)

project(tests VERSION ${CMAKE_PROJECT_VERSION})

add_definitions(-DPROJECT_ROOT="${CMAKE_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

include_directories(../library/include)

add_subdirectory(googletest)

set(SOURCES
    library.cpp
    serialization.cpp)

add_custom_command(
    OUTPUT generated/reflection.h
    COMMAND er_gen -p -c ${CMAKE_SOURCE_DIR}/tests/config.yaml
    DEPENDS data/tresholds.h data/various.h
    COMMENT "Generating reflection headers"
)

add_executable(${PROJECT_NAME} ${SOURCES} generated/reflection.h)

target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main)
target_link_libraries(${PROJECT_NAME} PRIVATE reflection)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
