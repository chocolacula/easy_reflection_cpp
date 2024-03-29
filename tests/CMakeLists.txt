cmake_minimum_required(VERSION 3.19)

project(tests VERSION ${CMAKE_PROJECT_VERSION})

add_definitions(-DPROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DREPEATS=1000)

disable_attribute_warnings()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

set(SOURCES
    random/random.cpp
    library.cpp
    tresholds.cpp
    various.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/reflection.cpp)

generate_reflection_code(
    CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/config.yaml
    OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated
    DEPENDS
        data/tresholds.h
        data/various.h)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/generated)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)

include_directories(../library/include)
target_link_libraries(${PROJECT_NAME} PRIVATE reflection)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
