include(cmake/embedded.cmake)
include(cmake/tests.cmake)

# Globally accessible executables/scripts
if(NOT STM32CUBEMX_BIN_PATH)
    # Default STM32CubeMX path
    if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
        set(STM32CUBEMX_BIN_PATH "C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeMX/STM32CubeMX.exe")
    elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
        set(STM32CUBEMX_BIN_PATH "/Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOs/STM32CubeMX")
    else()
        set(STM32CUBEMX_BIN_PATH "~/STM32CubeMX/STM32CubeMX")
    endif()
endif()

# Globally accessible targets
if("${PLATFORM}" STREQUAL "arm")
    # This target can be used to build the firmware binaries for all the boards
    add_custom_target(all_arm_binaries)
    add_compile_definitions(TARGET_EMBEDDED)
elseif("${PLATFORM}" STREQUAL "x86")
    # This target can be used to build the tests for all the boards
    add_custom_target(all_tests)
    add_compile_definitions(TARGET_TEST)
endif()

# Generate header file for commit message
set(GENERATE_COMMIT_INFO_SCRIPT_PY
${SCRIPTS_DIR}/code_generation/commit_info_gen/src/generate_commit_info.py)

function(commit_info_code_generation
    HEADER_OUTPUT_PATH
    SRC_OUTPUT_PATH
)
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
endfunction()



# Generate C code for sending/receiving CAN messages for BOARD_NAME
function(can_code_generation
    BOARD_NAME
    APP_CAN_TX_SRC_OUTPUT
    APP_CAN_TX_HEADER_OUTPUT
    IO_CAN_TX_SRC_OUTPUT
    IO_CAN_TX_HEADER_OUTPUT
    APP_CAN_RX_SRC_OUTPUT
    APP_CAN_RX_HEADER_OUTPUT
    IO_CAN_RX_SRC_OUTPUT
    IO_CAN_RX_HEADER_OUTPUT
    APP_CAN_UTILS_SRC_OUTPUT
    APP_CAN_UTILS_HEADER_OUTPUT
    APP_CAN_ALERTS_SRC_OUTPUT
    APP_CAN_ALERTS_HEADER_OUTPUT
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
        --board ${BOARD_NAME}
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
endfunction()

function(jsoncan_library
    LIB_NAME
    TARGET_NAME
    OUTPUT_DIR
    STM32_MCU
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
            ${CMAKE_SOURCE_DIR}/firmware/shared/src/app
            ${CMAKE_SOURCE_DIR}/firmware/shared/src/io
            ${CMAKE_SOURCE_DIR}/firmware/shared/src/hw
        )

        compile_embedded_lib(
            "${LIB_NAME}"
            "${CAN_SRCS}"
            "${CAN_INCLUDE_DIRS}"
            "${STM32_MCU}"
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
            ${CMAKE_SOURCE_DIR}/firmware/shared/src/app
            ${CMAKE_SOURCE_DIR}/firmware/shared/src/io
        )

        add_library(
            "${LIB_NAME}" STATIC 
            "${CAN_SRCS}"
        )
        target_include_directories("${LIB_NAME}" PUBLIC "${CAN_INCLUDE_DIRS}")
    endif()

endfunction()

# Generate STM32CubeMX driver code for BOARD_NAME using the given IOC_FILE in
# the directory where this function is called from.
function(cube_mx_code_generation
    BOARD_NAME
    IOC_FILE
)
    set(GENERATE_CUBE_CODE_SCRIPT_PY
        ${SCRIPTS_DIR}/utilities/generate_cube_code.py)
    set(FIX_FORMATTING_SCRIPT_PY
        ${SCRIPTS_DIR}/clang_format/fix_formatting.py)
    set(LOG4J_PROPERTIES "$ENV{HOME}/.stm32cubemx/log4j.properties")
    get_filename_component(IOC_DIR ${IOC_FILE} DIRECTORY)

    add_custom_command(
        OUTPUT ${IOC_FILE}.md5
        ${LOG4J_PROPERTIES}
        COMMENT "Generating Drivers for ${BOARD_NAME}"
        COMMAND ${PYTHON_COMMAND}
        ${GENERATE_CUBE_CODE_SCRIPT_PY}
        --board ${BOARD_NAME}
        --log4j_properties_output ${LOG4J_PROPERTIES}
        --ioc ${IOC_FILE}
        --codegen_output_dir ${IOC_DIR}
        --cube_bin ${STM32CUBEMX_BIN_PATH}

        # Create a MD5 hash of IOC_FILE for other build targets to depend on, so
        # this custom command is only executed when IOC_FILE.md5 changes.
        COMMAND ${PYTHON_COMMAND}
        ${SCRIPTS_DIR}/utilities/generate_md5_checksum.py
        ${IOC_FILE}
        ${IOC_FILE}.md5
        DEPENDS ${IOC_FILE}

        # Run clang-format because the STM32CubeMX-generated code is not
        # compliant to our clang-format rules.
        COMMAND ${PYTHON_COMMAND}
        ${FIX_FORMATTING_SCRIPT_PY}
        WORKING_DIRECTORY ${REPO_ROOT_DIR}
    )
endfunction()

function(compile_board_gtest_executable
    BOARD_NAME
    TEST_SRCS
    TEST_INCLUDE_DIRS
    ARM_BINARY_X86_COMPATIBLE_SRCS
    ARM_BINARY_INCLUDE_DIRS
)
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        set(TEST_EXECUTABLE_NAME "${BOARD_NAME}_test.exe")
    else()
        set(TEST_EXECUTABLE_NAME "${BOARD_NAME}_test")
    endif()

    list(APPEND BOARD_TEST_SRCS ${TEST_SRCS}
        ${ARM_BINARY_X86_COMPATIBLE_SRCS}
        ${SHARED_ARM_BINARY_X86_COMPATIBLE_SRCS}
    )
    list(APPEND BOARD_TEST_INCLUDE_DIRS ${TEST_INCLUDE_DIRS}
        ${ARM_BINARY_INCLUDE_DIRS}
        ${SHARED_ARM_BINARY_INCLUDE_DIRS}
        ${SHARED_GOOGLETEST_TEST_THIRD_PARTY_INCLUDE_DIRS}
        ${FIRMWARE_DIR}/shared/test_utils
    )
    compile_gtest_executable(
        "${TEST_EXECUTABLE_NAME}"
        "${BOARD_TEST_SRCS}"
        "${BOARD_TEST_INCLUDE_DIRS}"
    )
    target_link_libraries(${TEST_EXECUTABLE_NAME} "shared_fakes" )

    set(BOARD_FAKE_LIBRARY "${BOARD_NAME}_fakes")
    if(TARGET ${BOARD_FAKE_LIBRARY})
        target_link_libraries(${TEST_EXECUTABLE_NAME} ${BOARD_FAKE_LIBRARY})
    endif()

    add_dependencies(all_tests ${TEST_EXECUTABLE_NAME})
endfunction()

# Creates the Arm binary or tests for a board, depending on what CPU architecture
# this is being compiled for.
#
# @note The App and Io source files must be sorted in two platform-specific
# lists:
#
# 1. ARM_BINARY_X86_COMPATIBLE_SRCS - These can be compiled on x86
# 2. ARM_BINARY_X86_INCOMPATIBLE_SRCS - These can't be compiled on x86
#
# Every App file can be compiled on x86. However, the same is not true
# for Io files. Some Io files rely on STM headers (e.g. stm32f3xx.h) and
# thus can't be compiled on x86. The rest of the Io files, however, can
# be compiled on x86.
#
# These relationships can be summarized as follows:
#
# ARM_BINARY_X86_COMPATIBLE_SRCS = App source files + x86-compatible Io files
# ARM_BINARY_X86_INCOMPATIBLE_SRCS = Io sources files - x86-compatible Io files
#
# On the other hand, the App and Io include directories are always
# x86-compatible because they are paths and not actual files. We just have
# to place App and Io include directories in ARM_BINARY_INCLUDE_DIRS.
#
# BOARD_NAME - The name of the board, will be used to name the binary or tests
# IOC_FILE - The IOC file for the board
# ARM_BINARY_X86_COMPATIBLE_SRCS - Source files for the Arm binary that can be
# compiled on x86
# ARM_BINARY_X86_INCOMPATIBLE_SRCS - Source files for the Arm binary that
# can't be compiled on x86
# ARM_BINARY_INCLUDE_DIRS - The include directories for the Arm binary
# ARM_BINARY_THIRD_PARTY_SRCS - Third-party source files for the Arm binary
# that should have warnings suppressed
# ARM_BINARY_THIRD_PARTY_INCLUDE_DIRS - Third-party include directories for
# the Arm binary that should have
# warnings suppressed
# ARM_LINKER_SCRIPT - The Arm linker script for the board
# STM32_DEVICE_TYPE - The device name required for <stm32f3xx.h> to work
# GOOGLETEST_TEST_SRCS - Source files for the test programs
# GOOGLETEST_TEST_INCLUDE_DIRS - Include directories for the test programs
function(create_arm_binary_or_tests_for_board
    BOARD_NAME
    IOC_FILE
    ARM_BINARY_X86_COMPATIBLE_SRCS
    ARM_BINARY_X86_INCOMPATIBLE_SRCS
    ARM_BINARY_INCLUDE_DIRS
    ARM_BINARY_THIRD_PARTY_SRCS
    ARM_BINARY_THIRD_PARTY_INCLUDE_DIRS
    ARM_LINKER_SCRIPT
    STM32_DEVICE_TYPE
    SELECTED_ARM_SHARED_BINARY_SRCS
    GOOGLETEST_TEST_SRCS
    GOOGLETEST_TEST_INCLUDE_DIRS
)
    # Construct the paths for the generated CAN code
    set(APP_CAN_TX_SRC_FILE_NAME "App_CanTx.c")
    set(APP_CAN_TX_HEADER_FILE_NAME "App_CanTx.h")
    set(IO_CAN_TX_SRC_FILE_NAME "Io_CanTx.c")
    set(IO_CAN_TX_HEADER_FILE_NAME "Io_CanTx.h")
    set(APP_CAN_RX_SRC_FILE_NAME "App_CanRx.c")
    set(APP_CAN_RX_HEADER_FILE_NAME "App_CanRx.h")
    set(IO_CAN_RX_SRC_FILE_NAME "Io_CanRx.c")
    set(IO_CAN_RX_HEADER_FILE_NAME "Io_CanRx.h")
    set(APP_CAN_UTILS_SRC_FILE_NAME "App_CanUtils.c")
    set(APP_CAN_UTILS_HEADER_FILE_NAME "App_CanUtils.h")
    set(APP_CAN_ALERTS_HEADER_FILE_NAME "App_CanAlerts.h")
    set(APP_CAN_ALERTS_SRC_FILE_NAME "App_CanAlerts.c")

    set(BOARD_SPECIFIC_AUTOGENERATED_APP_SRC_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/Src/App")
    set(BOARD_SPECIFIC_AUTOGENERATED_APP_INCLUDE_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/Inc/App")
    set(BOARD_SPECIFIC_AUTOGENERATED_IO_SRC_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/Src/Io")
    set(BOARD_SPECIFIC_AUTOGENERATED_IO_INCLUDE_DIR
        "${CMAKE_CURRENT_BINARY_DIR}/Inc/Io")

    set(APP_CAN_TX_SRC_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_SRC_DIR}/${APP_CAN_TX_SRC_FILE_NAME}")
    set(APP_CAN_TX_HEADER_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_INCLUDE_DIR}/${APP_CAN_TX_HEADER_FILE_NAME}")
    set(IO_CAN_TX_SRC_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_IO_SRC_DIR}/${IO_CAN_TX_SRC_FILE_NAME}")
    set(IO_CAN_TX_HEADER_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_IO_INCLUDE_DIR}/${IO_CAN_TX_HEADER_FILE_NAME}")
    set(APP_CAN_RX_SRC_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_SRC_DIR}/${APP_CAN_RX_SRC_FILE_NAME}")
    set(APP_CAN_RX_HEADER_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_INCLUDE_DIR}/${APP_CAN_RX_HEADER_FILE_NAME}")
    set(IO_CAN_RX_SRC_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_IO_SRC_DIR}/${IO_CAN_RX_SRC_FILE_NAME}")
    set(IO_CAN_RX_HEADER_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_IO_INCLUDE_DIR}/${IO_CAN_RX_HEADER_FILE_NAME}")
    set(APP_CAN_UTILS_SRC_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_SRC_DIR}/${APP_CAN_UTILS_SRC_FILE_NAME}")
    set(APP_CAN_UTILS_INCLUDE_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_INCLUDE_DIR}/${APP_CAN_UTILS_HEADER_FILE_NAME}")
    set(APP_CAN_ALERTS_SRC_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_SRC_DIR}/${APP_CAN_ALERTS_SRC_FILE_NAME}")
    set(APP_CAN_ALERTS_INCLUDE_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_INCLUDE_DIR}/${APP_CAN_ALERTS_HEADER_FILE_NAME}")

    set(AUTOGENERATED_CAN_APP_SRCS
        "${APP_CAN_TX_SRC_FILE}"
        "${APP_CAN_RX_SRC_FILE}"
        "${APP_CAN_UTILS_SRC_FILE}"
        "${APP_CAN_ALERTS_SRC_FILE}"
    )
    set(AUTOGENERATED_CAN_IO_SRCS
        "${IO_CAN_TX_SRC_FILE}"
        "${IO_CAN_RX_SRC_FILE}"
    )
    set(AUTOGENERATED_CAN_APP_INCLUDE_DIRS
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_INCLUDE_DIR}")
    set(AUTOGENERATED_CAN_IO_INCLUDE_DIRS
        "${BOARD_SPECIFIC_AUTOGENERATED_IO_INCLUDE_DIR}")

    # Construct paths for generated commit info code 
    set(APP_COMMIT_INFO_HEADER_FILE_NAME "App_CommitInfo.h")
    set(APP_COMMIT_INFO_SRC_FILE_NAME "App_CommitInfo.c")
    
    set(APP_COMMIT_INFO_INCLUDE_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_INCLUDE_DIR}/${APP_COMMIT_INFO_HEADER_FILE_NAME}")
    set(APP_COMMIT_INFO_SRC_FILE
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_SRC_DIR}/${APP_COMMIT_INFO_SRC_FILE_NAME}")

    set(AUTOGENERATED_COMMIT_INFO_APP_INCLUDE_DIRS
        "${BOARD_SPECIFIC_AUTOGENERATED_APP_INCLUDE_DIR}")
    set(AUTOGENERATED_COMMIT_INFO_APP_SRCS
        "${APP_COMMIT_INFO_SRC_FILE}")

    list(APPEND ARM_BINARY_X86_COMPATIBLE_SRCS
        "${AUTOGENERATED_CAN_APP_SRCS}"
        "${AUTOGENERATED_COMMIT_INFO_APP_SRCS}")
    list(APPEND ARM_BINARY_INCLUDE_DIRS
        "${AUTOGENERATED_CAN_APP_INCLUDE_DIRS}"
        "${AUTOGENERATED_COMMIT_INFO_APP_INCLUDE_DIRS}")
    list(APPEND ARM_BINARY_X86_INCOMPATIBLE_SRCS
        "${AUTOGENERATED_CAN_IO_SRCS}")
    list(APPEND ARM_BINARY_INCLUDE_DIRS
        "${AUTOGENERATED_CAN_IO_INCLUDE_DIRS}")

    can_code_generation(
        "${BOARD_NAME}"
        "${APP_CAN_TX_SRC_FILE}"
        "${APP_CAN_TX_HEADER_FILE}"
        "${IO_CAN_TX_SRC_FILE}"
        "${IO_CAN_TX_HEADER_FILE}"
        "${APP_CAN_RX_SRC_FILE}"
        "${APP_CAN_RX_HEADER_FILE}"
        "${IO_CAN_RX_SRC_FILE}"
        "${IO_CAN_RX_HEADER_FILE}"
        "${APP_CAN_UTILS_SRC_FILE}"
        "${APP_CAN_UTILS_INCLUDE_FILE}"
        "${APP_CAN_ALERTS_SRC_FILE}"
        "${APP_CAN_ALERTS_INCLUDE_FILE}"
    )

    commit_info_code_generation(
        "${APP_COMMIT_INFO_INCLUDE_FILE}"
        "${APP_COMMIT_INFO_SRC_FILE}"
    )

    if("${PLATFORM}" STREQUAL "x86")
        add_compile_definitions(TARGET_TEST)
        compile_board_gtest_executable(
            "${BOARD_NAME}"
            "${GOOGLETEST_TEST_SRCS}"
            "${GOOGLETEST_TEST_INCLUDE_DIRS}"
            "${ARM_BINARY_X86_COMPATIBLE_SRCS}"
            "${ARM_BINARY_INCLUDE_DIRS}"
        )
    elseif("${PLATFORM}" STREQUAL "arm")
        add_compile_definitions(TARGET_EMBEDDED)
        cube_mx_code_generation(
            "${BOARD_NAME}"
            "${IOC_FILE}"
        )
        list(APPEND ARM_BINARY_SRCS
            ${ARM_BINARY_X86_COMPATIBLE_SRCS}
            ${ARM_BINARY_X86_INCOMPATIBLE_SRCS}
            ${APP_COMMIT_INFO_INCLUDE_FILE}
        )
        list(APPEND ARM_BINARY_INCLUDE_DIRS
            ${BOARD_SPECIFIC_AUTOGENERATED_APP_INCLUDE_DIR}
            ${BOARD_SPECIFIC_AUTOGENERATED_IO_INCLUDE_DIR}
        )
        compile_arm_binary(
            "${BOARD_NAME}"
            "${IOC_FILE}"
            "${ARM_BINARY_SRCS}"
            "${ARM_BINARY_INCLUDE_DIRS}"
            "${ARM_BINARY_THIRD_PARTY_SRCS}"
            "${ARM_BINARY_THIRD_PARTY_INCLUDE_DIRS}"
            "${ARM_LINKER_SCRIPT}"
            "${SELECTED_ARM_SHARED_BINARY_SRCS}"
            "${STM32_DEVICE_TYPE}"
        )
    endif()
endfunction()

## shared library
set(SHARED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shared") 
file(GLOB_RECURSE ALL_SHARED_APP_SRCS "${SHARED_DIR}/src/app/*.c")
file(GLOB_RECURSE ALL_SHARED_IO_SRCS "${SHARED_DIR}/src/io/*.c")
file(GLOB_RECURSE ALL_SHARED_HW_SRCS "${SHARED_DIR}/src/hw/*.c")

add_subdirectory(third_party)
add_subdirectory(shared)
add_subdirectory(thruna)