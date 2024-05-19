set(FIRMWARE_DIR "${CMAKE_SOURCE_DIR}/firmware")
set(LINKER_DIR "${FIRMWARE_DIR}/linker")
set(BOOT_DIR "${FIRMWARE_DIR}/boot")
set(SHARED_DIR "${FIRMWARE_DIR}/shared")
set(SHARED_EMBEDDED_DIR "${SHARED_DIR}/src")
set(SHARED_TEST_DIR "${SHARED_DIR}/test")
set(THIRD_PARTY_DIR "${FIRMWARE_DIR}/third_party")

set(SHARED_APP_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/app")
set(SHARED_IO_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/io")
set(SHARED_HW_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/hw")
set(SHARED_TEST_UTILS_INCLUDE_DIRS "${SHARED_DIR}/test_utils")

# Generate library with header file for commit message
function(commit_info_library
    BIND_TARGET
    LIB_NAME
    OUTPUT_PATH
    ARM_CORE
)
    commit_info_generate_sources(${BIND_TARGET} ${OUTPUT_PATH})
    if("${TARGET}" STREQUAL "deploy")
        embedded_library(
            "${LIB_NAME}"
            "${COMMIT_INFO_SRC}"
            "${COMMIT_INFO_INCLUDE_DIR}"
            "${ARM_CORE}"
            FALSE
        )
    elseif("${TARGET}" STREQUAL "test")
        get_filename_component(HEADER_DIR "${HEADER_OUTPUT_PATH}" DIRECTORY)
        add_library(
            "${LIB_NAME}" STATIC
            "${COMMIT_INFO_SRC}"
        )
        target_include_directories("${LIB_NAME}" PUBLIC "${HEADER_DIR}")
    endif()
endfunction()