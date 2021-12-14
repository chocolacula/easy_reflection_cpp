cmake_minimum_required(VERSION 3.10)

project(reflection VERSION ${CMAKE_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
include_directories(RE-flex/include)

set(SOURCES
        src/reflection/reflection.cpp
        src/err_helper.cpp
        src/variable/var.cpp
        src/variable/box.cpp
        src/serialization/json/json.cpp
        src/serialization/json/lexer_json.yy.cpp
        src/serialization/json/parser_json.cpp
        src/serialization/yaml/yaml.cpp
        src/serialization/yaml/lexer_yaml.yy.cpp
        src/serialization/yaml/parser_yaml.cpp
        src/serialization/binary/binary.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/RE-flex/lib/libreflex.a)
    message("make RE-flex library")
    execute_process(
        COMMAND ./build.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/library/RE-flex)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/RE-flex/lib/libreflex.a)