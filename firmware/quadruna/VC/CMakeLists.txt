set(ARM_CORE "cm7")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/VC.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_flash.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS "${SHARED_APP_SRCS}")
list(REMOVE_ITEM APP_SRCS "${SHARED_EMBEDDED_DIR}/app/App_SharedSetPeriodicCanSignals.c")
list(REMOVE_ITEM APP_SRCS "${SHARED_EMBEDDED_DIR}/app/App_SharedSignal.c")
list(REMOVE_ITEM APP_SRCS "${SHARED_EMBEDDED_DIR}/app/App_SharedWaitSignal.c")
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS "${SHARED_IO_SRCS}")
list(REMOVE_ITEM IO_SRCS "${SHARED_IO_INCLUDE_DIR}/Io_SharedAdc.c")
list(REMOVE_ITEM IO_SRCS "${SHARED_IO_INCLUDE_DIR}/Io_SharedSpi.c")
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS 
"${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
"${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
list(APPEND HW_SRCS "${SHARED_HW_SRCS}")
list(REMOVE_ITEM HW_SRCS "${SHARED_EMBEDDED_DIR}/hw/hw_sd.c")
list(REMOVE_ITEM HW_SRCS "${SHARED_EMBEDDED_DIR}/hw/hw_can.c")
list(REMOVE_ITEM HW_SRCS "${SHARED_EMBEDDED_DIR}/hw/hw_crc.c")
list(REMOVE_ITEM HW_SRCS "${SHARED_EMBEDDED_DIR}/hw/hw_flash.c")
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${SHARED_HW_INCLUDE_DIR}")

set(COMMIT_INFO_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.h")
set(COMMIT_INFO_SRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.c")
commit_info_library(
    "VC_commit_info"
    "${COMMIT_INFO_HEADER_PATH}"
    "${COMMIT_INFO_SRC_PATH}"
    "${ARM_CORE}"
)

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "VC_can"
        "VC"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
    )
    embedded_library(
        VC_can
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )
    generate_stm32cube_code(
        "VC"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        "stm32h7xx_hal_adc_ex.c"
        "stm32h7xx_hal_adc.c"
        "stm32h7xx_hal_cortex.c"
        "stm32h7xx_hal_dma_ex.c"
        "stm32h7xx_hal_dma.c"
        "stm32h7xx_hal_exti.c"
        "stm32h7xx_hal_fdcan.c"
        "stm32h7xx_hal_gpio.c"
        "stm32h7xx_hal_iwdg.c"
        "stm32h7xx_hal_pwr_ex.c"
        "stm32h7xx_hal_rcc_ex.c"
        "stm32h7xx_hal_rcc.c"
        "stm32h7xx_hal_tim_ex.c"
        "stm32h7xx_hal_tim.c"
        "stm32h7xx_hal_uart_ex.c"
        "stm32h7xx_hal_uart.c"
        "stm32h7xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32h733xx_cube_library(
        "VC_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    embedded_binary(
        "VC"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )
    target_link_libraries("VC_can" "VC_stm32cube")
    target_link_libraries("VC.elf" "VC_stm32cube" "VC_can" "VC_commit_info")

elseif("${TARGET}" STREQUAL "test")
    jsoncan_library(
        "VC_can"
        "VC"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
    )
    add_library(VC_can STATIC "${CAN_SRCS}")
    target_include_directories(VC_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_efuse.h"
    )
    create_fake_library(
        "VC_fakes"
        "${HEADERS_TO_FAKE}"
    )

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
        "VC_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("VC_test" "VC_can" "VC_commit_info" "VC_fakes" "shared_fakes")
endif()
