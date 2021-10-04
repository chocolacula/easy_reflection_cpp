cmake_minimum_required(VERSION 3.10)

project(reflection VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
include_directories(RE-flex/include)

set(SOURCES
        src/reflection.cpp
        src/err_helper.cpp
        src/variable/var.cpp
        src/variable/box.cpp
        src/serialization/parsing/parser_json.cpp
        src/serialization/parsing/parser_yaml.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/library/RE-flex/lib/libreflex.a)
    message("make RE-flex library")
    execute_process(
        COMMAND ./build.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/library/RE-flex)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/library/RE-flex/lib/libreflex.a)