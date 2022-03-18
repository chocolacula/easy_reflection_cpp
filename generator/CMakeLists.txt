cmake_minimum_required(VERSION 3.10)

project(er_gen VERSION ${CMAKE_PROJECT_VERSION})

add_definitions(-DVERSION="${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

include_directories(../library/include)

find_path(TCLAP_INCLUDE_DIRS "tclap/Arg.h")
include_directories(${TCLAP_INCLUDE_DIRS})

### LLVM and clang
find_package(LLVM REQUIRED)

message("found LLVM include directory at: \"" ${LLVM_INCLUDE_DIR} "\"")
message("found LLVM library directory at: \"" ${LLVM_LIBRARY_DIR} "\"")

include_directories(${LLVM_INCLUDE_DIR})
link_directories(${LLVM_LIBRARY_DIR})
###

find_package(inja CONFIG REQUIRED)

set(SOURCES
    main.cpp
    executable_name.cpp
    self_generated/reflection.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE reflection)
target_link_libraries(${PROJECT_NAME} PRIVATE pantor::inja)

option(LINK_CLANG_STATIC "Link LLVM and clang libraries statically" OFF)

if(LINK_CLANG_STATIC OR WIN32)
    message("clang libs will be linked statically")
    target_link_libraries(${PROJECT_NAME} PRIVATE
        LLVMDiff
        clangAST
        clangASTMatchers
        clangAnalysis
        clangBasic
        clangDriver
        clangEdit
        clangFrontend
        clangLex
        clangParse
        clangSema
        clangSerialization
        clangTooling
    )
else()
    message("clang libs will be linked dynamically")
    target_link_libraries(${PROJECT_NAME} PRIVATE LLVM clang-cpp)
endif()

# configure_file(config.yaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.yaml COPYONLY)
