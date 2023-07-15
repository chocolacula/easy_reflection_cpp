# Update git submodule.
# Arguments:
#   SUBMODULE - name of the updating submodule
function(update_submodule SUBMODULE)
    find_package(Git QUIET)

    if(Git_FOUND)
        message(STATUS "Updating git submodule ${SUBMODULE}")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${SUBMODULE}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL 0)
            message(FATAL_ERROR "Update failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endfunction()

# Generate code for reflection of user-defined types.
# Arguments:
#   CONFIG     - Path to the configuration file in YAML
#   OUTPUT_DIR - Directory where generated files will be placed
#   DEPENDS    - List of files that trigger code generation when changed
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

# Print variables that match the optional parameter.
# Arguments:
#   FILTER - print only variables matching this string, print all if it's empty
function(print_variables)
    set(FILTER ${ARGN})

    message("----------------------------------------------------------------")
    get_cmake_property(_variables VARIABLES)

    foreach (_var ${_variables})
        if("${_var}" MATCHES "${FILTER}")
            message("${_var}=${${_var}}")
        endif()
    endforeach()

    message("----------------------------------------------------------------")
endfunction()
