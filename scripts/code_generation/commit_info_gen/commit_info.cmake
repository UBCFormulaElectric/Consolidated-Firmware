# WE NEED bind_target in order to watch it to rebuild commitinfo
function(commit_info_register_library bind_target commit_info_directory)
    file(RELATIVE_PATH directory_location_relative ${CMAKE_SOURCE_DIR} ${commit_info_directory})
    message("📚 Registering commit info library ${bind_target} at ${directory_location_relative}")

    set(src_location "${commit_info_directory}/app_commitInfo.c")
    set(header_location "${commit_info_directory}/app_commitInfo.h")

    option(USE_COMMIT_INFO "Use commit info" ON) # ON, OFF OR MINIMAL (generates commitinfo only at generate time (old behaviour))
    if (${USE_COMMIT_INFO} STREQUAL "OFF")
        if(NOT EXISTS ${src_location} OR NOT EXISTS ${header_location})
            message(FATAL_ERROR "❌ commit_info file not found. Please add the '-DUSE_COMMIT_INFO' option")
        endif ()
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