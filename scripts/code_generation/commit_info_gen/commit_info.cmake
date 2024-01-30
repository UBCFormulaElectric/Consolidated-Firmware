# WE NEED bind_target in order to watch it to rebuild commitinfo
function(commit_info_register_library bind_target src_location header_location)
    file(RELATIVE_PATH header_location_relative ${CMAKE_SOURCE_DIR} ${header_location})
    message("📚 Registering commit info library ${bind_target} with header at ${header_location_relative}")

    option(USE_COMMIT_INFO "Use commit info" ON) # ON, OFF OR MINIMAL (generates commitinfo only at generate time (old behaviour))
    if (${USE_COMMIT_INFO} STREQUAL "OFF")
        return()
    endif()

    set(GENERATE_COMMIT_INFO_SCRIPT_PY ${SCRIPTS_DIR}/code_generation/commit_info_gen/src/generate_commit_info.py)
    if (${USE_COMMIT_INFO} STREQUAL "MINIMAL")
        execute_process(COMMAND ${PYTHON_COMMAND} ${GENERATE_COMMIT_INFO_SCRIPT_PY}
            --output-header ${header_location}
            --output-source ${src_location}
            WORKING_DIRECTORY ${REPO_ROOT_DIR})
        return ()
    endif ()

    add_custom_command( # we create this one so that it updates the file every build
        TARGET ${bind_target}
        COMMAND ${PYTHON_COMMAND} ${GENERATE_COMMIT_INFO_SCRIPT_PY}
        --output-header ${header_location}
        --output-source ${src_location}
        WORKING_DIRECTORY ${REPO_ROOT_DIR}
    )
    add_custom_command(
        OUTPUT ${header_location} ${src_location}
        COMMAND ${PYTHON_COMMAND} ${GENERATE_COMMIT_INFO_SCRIPT_PY}
        --output-header ${header_location}
        --output-source ${src_location}
        WORKING_DIRECTORY ${REPO_ROOT_DIR}
    )
endfunction()