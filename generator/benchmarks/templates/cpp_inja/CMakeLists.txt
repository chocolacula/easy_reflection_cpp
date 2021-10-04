cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0048 NEW)
project(inja_generator VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

include_directories(inja/third_party/include)
include_directories(inja/include)

set(SOURCES
	main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})