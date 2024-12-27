message("")
message("⚙️ [shared.cmake] Configuring shared variables and functions")
set(SHARED_CMAKE_INCLUDED TRUE)

set(FIRMWARE_DIR "${CMAKE_SOURCE_DIR}/firmware")
set(LINKER_DIR "${FIRMWARE_DIR}/linker")
set(BOOT_DIR "${FIRMWARE_DIR}/boot")
set(SHARED_DIR "${FIRMWARE_DIR}/shared")
set(SHARED_TEST_DIR "${SHARED_DIR}/test")
set(THIRD_PARTY_DIR "${FIRMWARE_DIR}/third_party")

# C shared code
set(SHARED_EMBEDDED_DIR "${SHARED_DIR}/src")
set(SHARED_APP_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/app")
set(SHARED_IO_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/io")
set(SHARED_HW_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/hw")
# C++ shared code
set(SHARED_EMBEDDED_DIR_CPP "${SHARED_DIR}/srcpp")
set(SHARED_APP_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/app")
set(SHARED_IO_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/io")
set(SHARED_HW_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/hw")

# Test Utils
set(SHARED_TEST_UTILS_INCLUDE_DIRS "${SHARED_DIR}/test_utils")

# Generate library with header file for commit message
message("  🔃 Registered commit_info_library() function")
function(commit_info_library
    BIND_TARGET
    LIB_NAME
    OUTPUT_PATH
)
    commit_info_generate_sources(${BIND_TARGET} ${OUTPUT_PATH})
    IF("${TARGET}" STREQUAL "binary")
        embedded_interface_library(
            "${LIB_NAME}"
            "${COMMIT_INFO_SRC}"
            "${COMMIT_INFO_INCLUDE_DIR}"
            FALSE
        )
    ELSEIF("${TARGET}" STREQUAL "test")
        get_filename_component(HEADER_DIR "${HEADER_OUTPUT_PATH}" DIRECTORY)
        # TODO make this an interface library as well
        add_library(
            "${LIB_NAME}" INTERFACE
            "${COMMIT_INFO_SRC}"
        )
        target_include_directories("${LIB_NAME}" INTERFACE "${HEADER_DIR}")
    ENDIF()
endfunction()

# Generates library ${CAR}_${BOARD}_jsoncan
message("  🔃 Registered jsoncan_library() function")
function(jsoncan_embedded_library BOARD CAR JSONCAN_DIR)
    jsoncan_sources(
            ${BOARD}
            ${JSONCAN_DIR}
            TRUE
            ${CAR}
    )
    embedded_interface_library(
            "${CAR}_${BOARD}_jsoncan"
            "${CAN_SRCS}"
            "${CAN_INCLUDE_DIRS}"
            TRUE
    )
endfunction()

function(jsoncan_library BOARD CAR JSONCAN_DIR)
    jsoncan_sources(
            ${BOARD}
            "${JSONCAN_DIR}"
            FALSE
            ${CAR}
    )
    add_library(
            "${CAR}_${BOARD}_jsoncan" STATIC
            "${CAN_SRCS}"
    )
    target_include_directories("${CAR}_${BOARD}_jsoncan" PUBLIC "${CAN_INCLUDE_DIRS}")
endfunction()