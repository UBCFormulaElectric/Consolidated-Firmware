# in particular, this is shared between embedded and tests
message("")
message("⚙️ [shared.cmake] Configuring shared variables and functions")
set(SHARED_CMAKE_INCLUDED TRUE)

set(FIRMWARE_DIR "${CMAKE_SOURCE_DIR}/firmware")
set(LINKER_DIR "${FIRMWARE_DIR}/linker")
set(BOOT_DIR "${FIRMWARE_DIR}/boot")
set(SHARED_DIR "${FIRMWARE_DIR}/shared")
set(THIRD_PARTY_DIR "${FIRMWARE_DIR}/third_party")
set(TEST_DIR "${FIRMWARE_DIR}/test")
set(SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/scripts")

# C shared code
set(SHARED_EMBEDDED_DIR "${SHARED_DIR}/src")
set(SHARED_APP_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/app")
set(SHARED_IO_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/io")
set(SHARED_HW_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/hw")
set(SHARED_FAKES_DIR "${SHARED_DIR}/src_fake")
# C++ shared code
set(SHARED_EMBEDDED_DIR_CPP "${SHARED_DIR}/srcpp")
set(SHARED_APP_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/app")
set(SHARED_IO_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/io")
set(SHARED_HW_INCLUDE_DIR_CPP "${SHARED_EMBEDDED_DIR_CPP}/hw")
# code sources
file(GLOB_RECURSE SHARED_APP_SRCS "${SHARED_APP_INCLUDE_DIR}/*.c" "${SHARED_APP_INCLUDE_DIR_CPP}/*.cpp")
file(GLOB_RECURSE SHARED_IO_SRCS "${SHARED_IO_INCLUDE_DIR}/*.c")
file(GLOB_RECURSE SHARED_HW_SRCS "${SHARED_HW_INCLUDE_DIR}/*.c")

# Test Utils
set(SHARED_TEST_UTILS_INCLUDE_DIRS "${SHARED_DIR}/test")

# Generate library with header file for commit message
message("  🔃 Registered commit_info_library() function")
function(commit_info_library
        BIND_TARGET
        LIB_NAME
        OUTPUT_PATH
)
    commit_info_generate_sources(${BIND_TARGET} ${OUTPUT_PATH})
    IF (${TARGET} STREQUAL "binary")
        embedded_interface_library(
                "${LIB_NAME}"
                "${COMMIT_INFO_SRC}"
                "${COMMIT_INFO_INCLUDE_DIR}"
                FALSE
        )
    ELSEIF (${TARGET} STREQUAL "test")
        get_filename_component(HEADER_DIR "${HEADER_OUTPUT_PATH}" DIRECTORY)
        add_library(${LIB_NAME} INTERFACE)
        target_sources(${LIB_NAME} INTERFACE ${COMMIT_INFO_SRC})
        target_include_directories("${LIB_NAME}" INTERFACE "${HEADER_DIR}")
    ENDIF ()
endfunction()

set(CAN_DIR ${CMAKE_SOURCE_DIR}/can_bus)

# Generates library ${CAR}_${BOARD}_jsoncan
message("  🔃 Registered jsoncan_library() function")
function(jsoncan_embedded_library BOARD CAR JSONCAN_DIR)
    jsoncan_sources(
            ${BOARD}
            ${JSONCAN_DIR}
            TRUE
            "${CAN_DIR}/dbcs/${CAR}.dbc"
            "${CAN_DIR}/${CAR}"
    )
    embedded_interface_library(
            "${CAR}_${BOARD}_jsoncan"
            "${CAN_SRCS}"
            "${CAN_INCLUDE_DIRS}"
            TRUE
    )
endfunction()

function(jsoncan_library BOARD CAR JSONCAN_DIR)
    set(JSONCAN_LIB "${CAR}_${BOARD}_jsoncan")
    set(JSONCAN_FAKES_LIB "${CAR}_${BOARD}_jsoncan_fakes")

    jsoncan_sources(
            ${BOARD}
            ${JSONCAN_DIR}
            FALSE
            "${CAN_DIR}/dbcs/${CAR}.dbc"
            "${CAN_DIR}/${CAR}"
    )
    add_library(${JSONCAN_LIB} INTERFACE)
    target_sources(${JSONCAN_LIB} INTERFACE ${CAN_SRCS})
    target_include_directories(${JSONCAN_LIB} INTERFACE "${CAN_INCLUDE_DIRS}")

    set(HEADERS_TO_FAKE
            "${JSONCAN_DIR}/io/io_canTx.h"
            "${JSONCAN_DIR}/io/io_canRx.h"
    )
    create_fake_library(
            "${JSONCAN_FAKES_LIB}"
            "${HEADERS_TO_FAKE}"
    )
    target_link_libraries(${JSONCAN_LIB} INTERFACE "${JSONCAN_FAKES_LIB}")
endfunction()
