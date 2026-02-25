message("")
message("⚙️ [test_libs.cmake] Configuring library wrappers")

message("  🔃 Registered jsoncan_library() function")
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
    no_checks("${CAN_SRCS}")
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

message("  🔃 Registered jsoncan_library_cpp() function")
function(jsoncan_library_cpp BOARD CAR JSONCAN_DIR)
    set(JSONCAN_LIB "${CAR}_${BOARD}_jsoncan")
    set(JSONCAN_FAKES_LIB "${CAR}_${BOARD}_jsoncan_fakes")

    jsoncan_sources_cpp(
            ${BOARD}
            ${JSONCAN_DIR}
            TRUE
            "${CAN_DIR}/dbcs/${CAR}.dbc"
            "${CAN_DIR}/${CAR}"
    )
    add_library(${JSONCAN_LIB} INTERFACE)
    target_sources(${JSONCAN_LIB} INTERFACE ${CAN_SRCS})
    no_checks("${CAN_SRCS}")
    target_include_directories(${JSONCAN_LIB} INTERFACE "${CAN_INCLUDE_DIRS}")
endfunction()

message("  🔃 Registered commit_info_library() function")
function(commit_info_library
        BIND_TARGET
        LIB_NAME
        OUTPUT_PATH
)
    commit_info_generate_sources(${BIND_TARGET} ${OUTPUT_PATH})
    get_filename_component(HEADER_DIR "${HEADER_OUTPUT_PATH}" DIRECTORY)
    add_library(${LIB_NAME} INTERFACE)
    target_sources(${LIB_NAME} INTERFACE ${COMMIT_INFO_SRC})
    target_include_directories("${LIB_NAME}" INTERFACE "${HEADER_DIR}")
endfunction()
