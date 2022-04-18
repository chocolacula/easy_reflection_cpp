cmake_minimum_required(VERSION 3.10)

project(tests VERSION ${CMAKE_PROJECT_VERSION})

add_definitions(-DPROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

include_directories(../library/include)

set(SOURCES
    random/random.cpp
    library.cpp
    tresholds.cpp
    various.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/reflection.cpp)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/generated/reflection.cpp
    COMMAND er_gen -p -c ${CMAKE_CURRENT_SOURCE_DIR}/config.yaml
    DEPENDS
        data/tresholds.h
        data/various.h
    COMMENT "Generating reflection headers")

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/generated)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)

target_link_libraries(${PROJECT_NAME} PRIVATE reflection)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
