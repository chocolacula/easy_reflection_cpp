# Update git submodule.
# Arguments:
#   SUBMODULE - name of the updating submodule
function(update_submodule SUBMODULE)
    find_package(Git QUIET)

    if (NOT Git_FOUND)
        message(ERROR "git not found!")
        return()
    endif()

    if(Git_FOUND)
        message(STATUS "Updating git submodule ${SUBMODULE}")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${SUBMODULE}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_RESULT)
        if(NOT GIT_RESULT EQUAL 0)
            message(FATAL_ERROR "Update failed with ${GIT_RESULT}")
        endif()
    endif()
endfunction()

# Apply git patch to a repository.
# Arguments:
#   FILENAME - relative path to the applying patch file
#   WORKDIR  - directory with a repo where to run applying the patch
function(apply_patch FILENAME WORKDIR)
    find_package(Git QUIET)

    if (NOT Git_FOUND)
        message(ERROR "git not found!")
        return()
    endif()

    set(FULL_WORKDIR ${CMAKE_CURRENT_SOURCE_DIR}/${WORKDIR})
    if(EXISTS ${FULL_WORKDIR}/patched)
        # Don't forget to add the file in a patch using `git add -N patched`.
        file(STRINGS ${FULL_WORKDIR}/patched IS_PATCHED LIMIT_COUNT 1)
    endif()

    message(STATUS "Applying patch to ${WORKDIR}")
    if ("${IS_PATCHED}" STREQUAL "true")
        message(STATUS "${WORKDIR} is already patched")
    else()
        execute_process(COMMAND ${GIT_EXECUTABLE} apply ${FILENAME}
                        WORKING_DIRECTORY ${FULL_WORKDIR}
                        RESULT_VARIABLE GIT_RESULT)
        if(GIT_RESULT EQUAL 0)
            message(STATUS "${WORKDIR} was patched!")
        else()
            message(ERROR "apply failed with ${GIT_RESULT}")
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

# Suppress compiler warnings for [[er:: ... ]] attribute names
function(disable_attribute_warnings)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options("-Wno-attributes")
    # matches Clang and AppleClang
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        add_compile_options("-Wno-unknown-attributes")
    endif()
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
