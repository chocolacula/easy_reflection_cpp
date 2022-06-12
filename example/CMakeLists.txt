cmake_minimum_required(VERSION 3.10)

project(example VERSION ${CMAKE_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

include_directories(../library/include)

set(SOURCES
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/reflection.cpp)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/generated/reflection.cpp
    COMMAND cd ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} && er_gen -p -c ${CMAKE_CURRENT_SOURCE_DIR}/config.yaml
    DEPENDS
        data/bicycle.h
        data/colors.h
    COMMENT "Generating reflection code")

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/generated)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE reflection)

# configure_file(config.yaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.yaml COPYONLY)
