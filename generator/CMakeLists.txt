cmake_minimum_required(VERSION 3.10)

project(er_gen VERSION ${CMAKE_PROJECT_VERSION})

add_definitions(-DVERSION="${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

include_directories(../library/include)
include_directories(inja/include)
include_directories(inja/third_party/include)
include_directories(tclap/include)

find_package(LLVM REQUIRED)
include_directories(${LLVM_INCLUDE_DIR})
link_directories(${LLVM_LIBRARY_DIR})

set(SOURCES
        main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE reflection)
target_link_libraries(${PROJECT_NAME} PRIVATE LLVM clang-cpp)

# configure_file(config.yaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.yaml COPYONLY)
