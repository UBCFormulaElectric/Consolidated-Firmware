# Inputs
# JSONCAN_PY_BOARD - Python board name
# OUTPUT_DIR - Output directory
# USE_IO - Whether to generate IO files
# CAR - Car name
#
# Returns
# CAN_SRCS - List of generated source files
# CAN_INCLUDE_DIRS - List of include directories
function(jsoncan_sources JSONCAN_PY_BOARD OUTPUT_DIR USE_IO CAR)
    file(RELATIVE_PATH OUTPUT_DIR_RELATIVE ${CMAKE_SOURCE_DIR} ${OUTPUT_DIR})
    message("📚 Registering JSONCAN sources for ${JSONCAN_PY_BOARD} at ${OUTPUT_DIR_RELATIVE} for ${CAR}")
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

    set(CAN_DIR ${REPO_ROOT_DIR}/can_bus)
    set(DBC_OUTPUT ${CAN_DIR}/dbcs/${CAR}.dbc)
    set(CAN_JSON_DIR ${CAN_DIR}/${CAR})
    file(GLOB_RECURSE CAN_JSON_SRCS ${CAN_JSON_DIR}/**/*.json)
    file(GLOB_RECURSE CAN_JSON_PY_SRCS ${SCRIPTS_DIR}/code_generation/jsoncan/**/*.py)
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
            COMMAND ${PYTHON_COMMAND}
            ${SCRIPTS_DIR}/code_generation/jsoncan/generate_can_from_json.py
            --board ${JSONCAN_PY_BOARD}
            --can_data_dir ${CAN_JSON_DIR}
            --output_dir ${OUTPUT_DIR}
            --dbc_output ${DBC_OUTPUT}
            DEPENDS ${CAN_JSON_SRCS} ${CAN_JSON_PY_SRCS}
            WORKING_DIRECTORY ${REPO_ROOT_DIR}
    )

    if (${USE_IO})
        set(CAN_SRCS
                ${APP_CAN_TX_SRC_OUTPUT}
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
                PARENT_SCOPE
        )
        set(CAN_INCLUDE_DIRS
                ${OUTPUT_DIR}/app
                ${OUTPUT_DIR}/io
                ${SHARED_APP_INCLUDE_DIR}
                ${SHARED_IO_INCLUDE_DIR}
                ${SHARED_HW_INCLUDE_DIR}
                PARENT_SCOPE
        )
    else ()
        set(CAN_SRCS
                ${APP_CAN_TX_SRC_OUTPUT}
                ${APP_CAN_TX_HEADER_OUTPUT}
                ${APP_CAN_RX_SRC_OUTPUT}
                ${APP_CAN_RX_HEADER_OUTPUT}
                ${APP_CAN_UTILS_SRC_OUTPUT}
                ${APP_CAN_UTILS_HEADER_OUTPUT}
                ${APP_CAN_ALERTS_SRC_OUTPUT}
                ${APP_CAN_ALERTS_HEADER_OUTPUT}
                ${APP_CAN_DATA_CAPTURE_SRC_OUTPUT}
                ${APP_CAN_DATA_CAPTURE_HEADER_OUTPUT}
                PARENT_SCOPE
        )
        set(CAN_INCLUDE_DIRS
                ${OUTPUT_DIR}/app
                ${SHARED_APP_INCLUDE_DIR}
                PARENT_SCOPE
        )
    endif ()
endfunction()