# Print variables that match the optional parameter.
# Arguments:
#   FILTER if it's empty, print all variables
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
