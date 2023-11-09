set(FIRMWARE_DIR "${CMAKE_SOURCE_DIR}/firmware")
set(SHARED_DIR "${FIRMWARE_DIR}/shared")
set(SHARED_EMBEDDED_DIR "${SHARED_DIR}/src")
set(SHARED_TEST_DIR "${SHARED_DIR}/test")
set(THIRD_PARTY_DIR "${FIRMWARE_DIR}/third_party")

set(SHARED_APP_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/app")
set(SHARED_IO_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/io")
set(SHARED_HW_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/hw")
set(SHARED_TEST_UTILS_INCLUDE_DIRS "${SHARED_DIR}/test_utils")

file(GLOB_RECURSE SHARED_APP_SRCS "${SHARED_APP_INCLUDE_DIR}/*.c")
file(GLOB_RECURSE SHARED_IO_SRCS "${SHARED_IO_INCLUDE_DIR}/*.c")
file(GLOB_RECURSE SHARED_HW_SRCS "${SHARED_HW_INCLUDE_DIR}/*.c")

# Generate library with header file for commit message
function(commit_info_library
    LIB_NAME
    HEADER_OUTPUT_PATH
    SRC_OUTPUT_PATH
    ARM_CORE
)
    set(GENERATE_COMMIT_INFO_SCRIPT_PY
        ${SCRIPTS_DIR}/code_generation/commit_info_gen/src/generate_commit_info.py)
    add_custom_command(
        OUTPUT ${HEADER_OUTPUT_PATH} ${SRC_OUTPUT_PATH}
        COMMAND ${PYTHON_COMMAND}
        ${GENERATE_COMMIT_INFO_SCRIPT_PY}
        --output-header
        ${HEADER_OUTPUT_PATH}
        --output-source
        ${SRC_OUTPUT_PATH}
        WORKING_DIRECTORY ${REPO_ROOT_DIR}
    )

    if("${TARGET}" STREQUAL "arm")
        embedded_library(
            "${LIB_NAME}"
            "${SRC_OUTPUT_PATH}"
            "${HEADER_OUTPUT_PATH}"
            "${ARM_CORE}"
            FALSE
        )
    elseif("${TARGET}" STREQUAL "x86")
        get_filename_component(HEADER_DIR "${HEADER_OUTPUT_PATH}" DIRECTORY)
        add_library(
            "${LIB_NAME}" STATIC
            "${SRC_OUTPUT_PATH}"
        )
        target_include_directories("${LIB_NAME}" PUBLIC "${HEADER_DIR}")
    endif()
endfunction()