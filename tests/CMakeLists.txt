cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0048 NEW)
project(test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(googletest)

include_directories(../library/include)
include_directories(../googletest/googletest/include)

set(SOURCES
	library.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} gtest_main)