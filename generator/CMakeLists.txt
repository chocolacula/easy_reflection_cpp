cmake_minimum_required(VERSION 3.19)

project(er_gen VERSION ${CMAKE_PROJECT_VERSION})

if(MSVC)
    add_compile_options("$<$<NOT:$<CONFIG:DEBUG>>:/Ox>")
else()
    add_compile_options("$<$<NOT:$<CONFIG:DEBUG>>:-O3>")
endif()

disable_attribute_warnings()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

set(SOURCES
    main.cpp
    parser/parser_cpp.cpp
    tools/files.cpp
    self_generated/reflection.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION="${PROJECT_VERSION}")

find_path(TCLAP_INCLUDE_DIRS "tclap/Arg.h")
include_directories(${TCLAP_INCLUDE_DIRS})

include_directories(../library/include)
target_link_libraries(${PROJECT_NAME} PRIVATE reflection)

find_package(inja CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE pantor::inja)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE version)
endif()

### LLVM and Clang ###
update_submodule(llvm-project)
apply_patch(../attribute.patch llvm-project)

set(LLVM_ENABLE_RTTI ON)
set(LLVM_INCLUDE_BENCHMARKS OFF)
set(LLVM_ENABLE_PROJECTS "clang")

add_subdirectory(llvm-project/llvm EXCLUDE_FROM_ALL)

include_directories(
  llvm-project/llvm/include
  ${CMAKE_CURRENT_BINARY_DIR}/llvm-project/llvm/include
  ${CMAKE_CURRENT_BINARY_DIR}/llvm-project/utils/bazel/llvm-project-overlay/llvm/include
  llvm-project/clang/include
  ${CMAKE_CURRENT_BINARY_DIR}/llvm-project/llvm/tools/clang/include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    clangAST
    clangASTMatchers
    clangFrontend
    clangLex
    clangTooling)

# configure_file(config.yaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.yaml COPYONLY)
