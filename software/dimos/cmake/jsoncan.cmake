function(jsoncan_library LIB_NAME TARGET_NAME OUTPUT_DIR)
    message("📚 Creating JSONCAN Library ${LIB_NAME} to ${OUTPUT_DIR}")
    set(APP_CAN_TX_SRC_OUTPUT "${OUTPUT_DIR}/app/App_CanTx.c")
    set(APP_CAN_TX_HEADER_OUTPUT "${OUTPUT_DIR}/app/App_CanTx.h")
    set(IO_CAN_TX_SRC_OUTPUT "${OUTPUT_DIR}/io/Io_CanTx.c")
    set(IO_CAN_TX_HEADER_OUTPUT "${OUTPUT_DIR}/io/Io_CanTx.h")
    set(APP_CAN_RX_SRC_OUTPUT "${OUTPUT_DIR}/app/App_CanRx.c")
    set(APP_CAN_RX_HEADER_OUTPUT "${OUTPUT_DIR}/app/App_CanRx.h")
    set(IO_CAN_RX_SRC_OUTPUT "${OUTPUT_DIR}/io/Io_CanRx.c")
    set(IO_CAN_RX_HEADER_OUTPUT "${OUTPUT_DIR}/io/Io_CanRx.h")
    set(APP_CAN_UTILS_SRC_OUTPUT "${OUTPUT_DIR}/app/App_CanUtils.c")
    set(APP_CAN_UTILS_HEADER_OUTPUT "${OUTPUT_DIR}/app/App_CanUtils.h")
    set(APP_CAN_ALERTS_SRC_OUTPUT "${OUTPUT_DIR}/app/App_CanAlerts.c")
    set(APP_CAN_ALERTS_HEADER_OUTPUT "${OUTPUT_DIR}/app/App_CanAlerts.h")

    set(CAN_DIR ${REPO_ROOT_DIR}/can_bus)
    set(DBC_OUTPUT ${CAN_DIR}/dbcs/CanMsgs.dbc)
    set(CAN_JSON_DIR ${CAN_DIR}/json)
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
            COMMAND ${PYTHON_COMMAND}
            ${SCRIPTS_DIR}/code_generation/jsoncan/generate_can_from_json.py
            --board ${TARGET_NAME} # TODO Check
            --can_data_dir ${CAN_JSON_DIR}
            --app_can_tx_header_output ${APP_CAN_TX_HEADER_OUTPUT}
            --app_can_tx_source_output ${APP_CAN_TX_SRC_OUTPUT}
            --io_can_tx_header_output ${IO_CAN_TX_HEADER_OUTPUT}
            --io_can_tx_source_output ${IO_CAN_TX_SRC_OUTPUT}
            --app_can_rx_header_output ${APP_CAN_RX_HEADER_OUTPUT}
            --app_can_rx_source_output ${APP_CAN_RX_SRC_OUTPUT}
            --io_can_rx_header_output ${IO_CAN_RX_HEADER_OUTPUT}
            --io_can_rx_source_output ${IO_CAN_RX_SRC_OUTPUT}
            --app_can_utils_header_output ${APP_CAN_UTILS_HEADER_OUTPUT}
            --app_can_utils_source_output ${APP_CAN_UTILS_SRC_OUTPUT}
            --app_can_alerts_header_output ${APP_CAN_ALERTS_HEADER_OUTPUT}
            --app_can_alerts_source_output ${APP_CAN_ALERTS_SRC_OUTPUT}
            --dbc_output ${DBC_OUTPUT}
            DEPENDS ${CAN_JSON_SRCS} ${CAN_JSON_PY_SRCS}
            WORKING_DIRECTORY ${REPO_ROOT_DIR}
    )

    set(CAN_SRCS
            ${APP_CAN_TX_SRC_OUTPUT}
            ${APP_CAN_TX_HEADER_OUTPUT}
            ${IO_CAN_TX_SRC_OUTPUT} #UNIQUE
            ${IO_CAN_TX_HEADER_OUTPUT} #UNIQUE
            ${APP_CAN_RX_SRC_OUTPUT}
            ${APP_CAN_RX_HEADER_OUTPUT}
            ${IO_CAN_RX_SRC_OUTPUT} #UNIQUE
            ${IO_CAN_RX_HEADER_OUTPUT} #UNIQUE
            ${APP_CAN_UTILS_SRC_OUTPUT}
            ${APP_CAN_UTILS_HEADER_OUTPUT}
            ${APP_CAN_ALERTS_SRC_OUTPUT}
            ${APP_CAN_ALERTS_HEADER_OUTPUT}
    )
    set(CAN_INCLUDE_DIRS
            ${OUTPUT_DIR}/app
            ${OUTPUT_DIR}/io # For IO
            ${SHARED_APP_INCLUDE_DIR}
            ${SHARED_IO_INCLUDE_DIR} # For IO
            ${SHARED_HW_INCLUDE_DIR} # For IO
    )

    add_library("${LIB_NAME}" STATIC ${CAN_SRCS})
    message("Adding CAN Library ${LIB_NAME} with sources ${CAN_SRCS}")
    target_include_directories(${LIB_NAME} PUBLIC ${CAN_INCLUDE_DIRS})
    message("Adding CAN Library ${LIB_NAME} with include dirs ${CAN_INCLUDE_DIRS}")
endfunction()