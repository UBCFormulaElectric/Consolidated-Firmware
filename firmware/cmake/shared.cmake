set(FIRMWARE_DIR "${CMAKE_SOURCE_DIR}/firmware")
SET(SHARED_DIR "${FIRMWARE_DIR}/shared")
SET(SHARED_EMBEDDED_DIR "${SHARED_DIR}/src")
SET(SHARED_TEST_DIR "${SHARED_DIR}/test")
SET(THIRD_PARTY_DIR "${FIRMWARE_DIR}/third_party")

set(SHARED_APP_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/app")
set(SHARED_IO_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/io")
set(SHARED_HW_INCLUDE_DIR "${SHARED_EMBEDDED_DIR}/hw")
set(SHARED_TEST_UTILS_INCLUDE_DIRS "${SHARED_DIR}/test_utils")

file(GLOB_RECURSE SHARED_APP_SRCS "${SHARED_APP_INCLUDE_DIR}/*.c")
file(GLOB_RECURSE SHARED_IO_SRCS "${SHARED_IO_INCLUDE_DIR}/*.c")
file(GLOB_RECURSE SHARED_HW_SRCS "${SHARED_HW_INCLUDE_DIR}/*.c")

function(jsoncan_library
    LIB_NAME
    TARGET_NAME
    OUTPUT_DIR
    ARM_CORE
)
    set(CAN_DIR ${REPO_ROOT_DIR}/can_bus)
    set(CAN_JSON_DIR ${CAN_DIR}/json)
    file(GLOB_RECURSE CAN_JSON_SRCS
        ${CAN_JSON_DIR}/**/*.json
    )
    set(DBC_OUTPUT ${CAN_DIR}/dbcs/CanMsgs.dbc)
    file(GLOB_RECURSE CAN_JSON_PY_SRCS
        ${SCRIPTS_DIR}/code_generation/jsoncan/**/*.py
    )

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
        --board ${TARGET_NAME}
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

    if("${PLATFORM}" STREQUAL "arm")
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
        )
        set(CAN_INCLUDE_DIRS
            ${OUTPUT_DIR}/app
            ${OUTPUT_DIR}/io
            ${SHARED_APP_INCLUDE_DIR}
            ${SHARED_IO_INCLUDE_DIR}
            ${SHARED_HW_INCLUDE_DIR}
        )
        embedded_library(
            "${LIB_NAME}"
            "${CAN_SRCS}"
            "${CAN_INCLUDE_DIRS}"
            "${ARM_CORE}"
            TRUE
        )

    elseif("${PLATFORM}" STREQUAL "x86")
        set(CAN_SRCS
            ${APP_CAN_TX_SRC_OUTPUT}
            ${APP_CAN_TX_HEADER_OUTPUT}
            ${APP_CAN_RX_SRC_OUTPUT}
            ${APP_CAN_RX_HEADER_OUTPUT}
            ${APP_CAN_UTILS_SRC_OUTPUT}
            ${APP_CAN_UTILS_HEADER_OUTPUT}
            ${APP_CAN_ALERTS_SRC_OUTPUT}
            ${APP_CAN_ALERTS_HEADER_OUTPUT}
        )
        set(CAN_INCLUDE_DIRS
            ${OUTPUT_DIR}/app
            ${SHARED_APP_INCLUDE_DIRS}
        )

        add_library(
            "${LIB_NAME}" STATIC
            "${CAN_SRCS}"
        )
        target_include_directories("${LIB_NAME}" PUBLIC "${CAN_INCLUDE_DIRS}")
    endif()
endfunction()

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

    if("${PLATFORM}" STREQUAL "arm")
        embedded_library(
            "${LIB_NAME}"
            "${SRC_OUTPUT_PATH}"
            "${HEADER_OUTPUT_PATH}"
            "${ARM_CORE}"
            FALSE
        )
    elseif("${PLATFORM}" STREQUAL "x86")
        get_filename_component(HEADER_DIR "${HEADER_OUTPUT_PATH}" DIRECTORY)
        add_library(
            "${LIB_NAME}" STATIC
            "${SRC_OUTPUT_PATH}"
        )
        target_include_directories("${LIB_NAME}" PUBLIC "${HEADER_DIR}")
    endif()
endfunction()