# Generate code for reflection of user-defined types.
# Arguments:
#   CONFIG      Path to the configuration file in YAML
#   OUTPUT_DIR  Directory where generated files will be placed
#   DEPENDS     List of files that trigger code generation when changed
function(generate_reflection_code)
    set(_prefix GEN)
    set(_flags)
    set(_single_arg CONFIG OUTPUT_DIR)
    set(_multi_args DEPENDS)

    include(CMakeParseArguments)
    cmake_parse_arguments(
        ${_prefix}
        "${_flags}"
        "${_single_arg}"
        "${_multi_args}"
        ${ARGN})

    if(MSVC)
        set(GENERATOR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/er_gen.exe)
    else()
        set(GENERATOR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/er_gen)
    endif()

    add_custom_command(
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        OUTPUT  ${GEN_OUTPUT_DIR}/reflection.cpp
        COMMAND ${GENERATOR} -p -c ${GEN_CONFIG}
        DEPENDS ${GEN_DEPENDS}
        COMMENT "Generating reflection code")
endfunction()
