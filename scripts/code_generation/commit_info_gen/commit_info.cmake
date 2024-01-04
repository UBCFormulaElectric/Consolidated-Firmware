# WWW
function(commit_info_register_library src_location header_location)
    set(GENERATE_COMMIT_INFO_SCRIPT_PY
            ${SCRIPTS_DIR}/code_generation/commit_info_gen/src/generate_commit_info.py)
    add_custom_command(
            OUTPUT ${header_location} ${src_location}
            COMMAND ${PYTHON_COMMAND} ${GENERATE_COMMIT_INFO_SCRIPT_PY}
            --output-header ${header_location}
            --output-source ${src_location}
            WORKING_DIRECTORY ${REPO_ROOT_DIR}
    )
endfunction()