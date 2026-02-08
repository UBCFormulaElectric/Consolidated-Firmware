# Inputs
# JSONCAN_PY_BOARD - Python board name
# OUTPUT_DIR - Output directory
# USE_IO - Whether to generate IO files
# CAR - Car name
#
# Returns
# CAN_SRCS - List of generated source files
# CAN_INCLUDE_DIRS - List of include directories
function(jsoncan_sources JSONCAN_PY_BOARD OUTPUT_DIR USE_IO DBC_OUTPUT CAN_JSON_DIR)
    file(RELATIVE_PATH OUTPUT_DIR_RELATIVE ${CMAKE_SOURCE_DIR} ${OUTPUT_DIR})
    message("  📚 [jsoncan.cmake, jsoncan_source()] Registering JSONCAN sources for ${JSONCAN_PY_BOARD} at ${OUTPUT_DIR_RELATIVE} for ${CAR}")
    set(APP_CAN_TX_SRC_OUTPUT "${OUTPUT_DIR}/app/app_canTx.c")
    set(APP_CAN_TX_HEADER_OUTPUT "${OUTPUT_DIR}/app/app_canTx.h")
    set(IO_CAN_TX_SRC_OUTPUT "${OUTPUT_DIR}/io/io_canTx.c")
    set(IO_CAN_TX_HEADER_OUTPUT "${OUTPUT_DIR}/io/io_canTx.h")
    set(APP_CAN_RX_SRC_OUTPUT "${OUTPUT_DIR}/app/app_canRx.c")
    set(APP_CAN_RX_HEADER_OUTPUT "${OUTPUT_DIR}/app/app_canRx.h")
    set(IO_CAN_RX_SRC_OUTPUT "${OUTPUT_DIR}/io/io_canRx.c")
    set(IO_CAN_RX_HEADER_OUTPUT "${OUTPUT_DIR}/io/io_canRx.h")
    set(APP_CAN_UTILS_SRC_OUTPUT "${OUTPUT_DIR}/app/app_canUtils.c")
    set(APP_CAN_UTILS_HEADER_OUTPUT "${OUTPUT_DIR}/app/app_canUtils.h")
    set(APP_CAN_ALERTS_SRC_OUTPUT "${OUTPUT_DIR}/app/app_canAlerts.c")
    set(APP_CAN_ALERTS_HEADER_OUTPUT "${OUTPUT_DIR}/app/app_canAlerts.h")
    set(APP_CAN_DATA_CAPTURE_SRC_OUTPUT "${OUTPUT_DIR}/app/app_canDataCapture.c")
    set(APP_CAN_DATA_CAPTURE_HEADER_OUTPUT "${OUTPUT_DIR}/app/app_canDataCapture.h")
    set(IO_CAN_REROUTE_SRC_OUTPUT "${OUTPUT_DIR}/io/io_canReroute.c")
    set(IO_CAN_REROUTE_HEADER_OUTPUT "${OUTPUT_DIR}/io/io_canReroute.h")

    file(GLOB_RECURSE CAN_JSON_SRCS ${CAN_JSON_DIR}/**/*.json)

    add_custom_command(
            OUTPUT ${APP_CAN_TX_SRC_OUTPUT}
            ${APP_CAN_TX_HEADER_OUTPUT}
            ${IO_CAN_TX_SRC_OUTPUT}
            ${IO_CAN_TX_HEADER_OUTPUT}
            ${APP_CAN_RX_SRC_OUTPUT}
            ${APP_CAN_RX_HEADER_OUTPUT}
            ${IO_CAN_RX_SRC_OUTPUT}
            ${IO_CAN_RX_HEADER_OUTPUT}
            ${APP_CAN_UTILS_SRC_OUTPUT}
            ${APP_CAN_UTILS_HEADER_OUTPUT}
            ${APP_CAN_ALERTS_SRC_OUTPUT}
            ${APP_CAN_ALERTS_HEADER_OUTPUT}
            ${APP_CAN_DATA_CAPTURE_SRC_OUTPUT}
            ${APP_CAN_DATA_CAPTURE_HEADER_OUTPUT}
            ${IO_CAN_REROUTE_SRC_OUTPUT}
            ${IO_CAN_REROUTE_HEADER_OUTPUT}
            COMMAND cargo run --release --bin cppcodegen --
            --board ${JSONCAN_PY_BOARD}
            --can_data_dir ${CAN_JSON_DIR}
            --output_dir ${OUTPUT_DIR}
            --dbc_output ${DBC_OUTPUT}
            DEPENDS ${CAN_JSON_SRCS} ${CAN_JSON_PY_SRCS}
            WORKING_DIRECTORY ${SCRIPTS_DIR}/code_generation/jsoncan-rust
    )


    IF (${USE_IO})
        set(CAN_SRCS
                ${APP_CAN_TX_SRC_OUTPUT}
                ${IO_CAN_TX_SRC_OUTPUT}
                ${APP_CAN_RX_SRC_OUTPUT}
                ${IO_CAN_RX_SRC_OUTPUT}
                ${APP_CAN_UTILS_SRC_OUTPUT}
                ${APP_CAN_ALERTS_SRC_OUTPUT}
                ${APP_CAN_DATA_CAPTURE_SRC_OUTPUT}
                ${IO_CAN_REROUTE_SRC_OUTPUT}
                PARENT_SCOPE
        )
    ELSE ()
        set(CAN_SRCS
                ${APP_CAN_TX_SRC_OUTPUT}
                ${APP_CAN_RX_SRC_OUTPUT}
                ${APP_CAN_UTILS_SRC_OUTPUT}
                ${APP_CAN_ALERTS_SRC_OUTPUT}
                ${APP_CAN_DATA_CAPTURE_SRC_OUTPUT}
                PARENT_SCOPE
        )
    ENDIF ()
    set(CAN_INCLUDE_DIRS
            ${OUTPUT_DIR}/app
            ${OUTPUT_DIR}/io
            PARENT_SCOPE
    )
endfunction()


# post build function to calculate bus load should print the bus load
# as we are planning to change the bitrates in the future so we can add bit rate as a parameter here

function(log_bus_load CAR)
    # Define CAN directory based on repository root directory
    set(CAN_DIR "${CMAKE_SOURCE_DIR}/can_bus")
    set(CAN_JSON_DIR "${CAN_DIR}/${CAR}")
    message("  📚 [jsoncan.cmake, log_bus_load()] Registering CAN bus load calculation for ${CAR}")
    add_custom_target(
            can_bus_load_${CAR}
            COMMAND cargo run --release --bin busload --
            --can_data_dir "${CAN_JSON_DIR}"
            WORKING_DIRECTORY ${SCRIPTS_DIR}/code_generation/jsoncan-rust
            COMMENT "Calculating CAN bus load using JSON CAN data for ${CAR}"
    )
endfunction()

message("")
message("Configuring JSONCAN")
log_bus_load("quintuna")
