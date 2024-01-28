# WE NEED bind_target in order to watch it to rebuild commitinfo
function(commit_info_register_library bind_target src_location header_location)
    option(USE_COMMIT_INFO "Use commit info" ON)
    if (NOT USE_COMMIT_INFO)
        return()
    endif()
    file(RELATIVE_PATH header_location_relative ${CMAKE_SOURCE_DIR} ${header_location})
    message("📚 Registering commit info library ${bind_target} with header at ${header_location_relative}")
    set(GENERATE_COMMIT_INFO_SCRIPT_PY ${SCRIPTS_DIR}/code_generation/commit_info_gen/src/generate_commit_info.py)
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