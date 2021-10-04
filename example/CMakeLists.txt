cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0048 NEW)
project(example VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

include_directories(../library/include)

set(SOURCES
    main.cpp)

add_custom_command(
    OUTPUT generated/reflection.h
    COMMAND er_gen -c ${CMAKE_SOURCE_DIR}/example/config.yaml
    DEPENDS data/bicycle.h data/color.h
    COMMENT "Generating reflection headers"
)

add_executable(${PROJECT_NAME} ${SOURCES} generated/reflection.h)

target_link_libraries(${PROJECT_NAME} PRIVATE reflection)

# configure_file(config.yaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.yaml COPYONLY)
