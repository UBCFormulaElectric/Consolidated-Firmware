# WE NEED bind_target in order to watch it to rebuild commitinfo
function(commit_info_generate_sources bind_target commit_info_directory)
    set(header_location "${commit_info_directory}/app_commitInfo.hpp")
    set(GENERATE_COMMIT_INFO_SCRIPT_PY ${SCRIPTS_DIR}/code_generation/commit_info_gen/src/generate_commit_info.py)

    set(COMMIT_INFO_INCLUDE_DIR ${commit_info_directory} PARENT_SCOPE)

    option(USE_COMMIT_INFO "Use commit info" ON) # ON, OFF OR MINIMAL (generates commitinfo only at generate time (old behaviour))
    # check if the file exists, if not generate it now
    IF (NOT EXISTS ${header_location})
        message("  📚 [commit_info.cmake, commit_info_generate_sources()] Commit info files not found, generating now...")
        execute_process(
                COMMAND ${PYTHON_COMMAND} ${GENERATE_COMMIT_INFO_SCRIPT_PY}
                --output-header ${header_location}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND_ERROR_IS_FATAL ANY
        )
        message("  📚 [commit_info.cmake, commit_info_generate_sources()] Generated commit info files at ${directory_location_relative}")
    ENDIF ()
    IF (${USE_COMMIT_INFO} STREQUAL "OFF")
        return()
    ENDIF ()

    file(RELATIVE_PATH directory_location_relative ${CMAKE_SOURCE_DIR} ${commit_info_directory})
    message("  📚 [commit_info.cmake, commit_info_generate_sources()] Registering commit info library ${bind_target}")

    IF (${USE_COMMIT_INFO} STREQUAL "MINIMAL")
        add_custom_command(
                OUTPUT ${header_location}
                COMMAND ${PYTHON_COMMAND} ${GENERATE_COMMIT_INFO_SCRIPT_PY}
                --output-header ${header_location}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        message("  📚 [commit_info.cmake, commit_info_generate_sources()] Registered rules for files at ${directory_location_relative}")
    ELSEIF (${USE_COMMIT_INFO} STREQUAL "ON")
        add_custom_command( # we create this one so that it updates the file every build
                TARGET ${bind_target}
                COMMAND ${PYTHON_COMMAND} ${GENERATE_COMMIT_INFO_SCRIPT_PY}
                --output-header ${header_location}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                PRE_BUILD
        )
        message("  📚 [commit_info.cmake, commit_info_generate_sources()] Registered bind target for ${bind_target}")
    ELSE ()
        message(FATAL_ERROR "❌ Unknown option for USE_COMMIT_INFO: ${USE_COMMIT_INFO}. Valid options are: ON, OFF, MINIMAL")
    ENDIF ()
endfunction()