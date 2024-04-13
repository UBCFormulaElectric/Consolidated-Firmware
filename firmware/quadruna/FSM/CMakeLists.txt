set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/FSM.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

# app
file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS 
    "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
    "${SHARED_APP_INCLUDE_DIR}/app_rangeCheck.c"
    "${SHARED_APP_INCLUDE_DIR}/app_signal.c"
    "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
    "${SHARED_APP_INCLUDE_DIR}/app_heartbeatMonitor.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_QUADRUNA_DIR}")

# io
file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c"
    "${SHARED_IO_INCLUDE_DIR}/io_time.c"
    "${SHARED_IO_INCLUDE_DIR}/io_chimera.c"
    "${SHARED_IO_INCLUDE_DIR}/io_mechanicalLoad.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_QUADRUNA_DIR}")

# hw
file(GLOB_RECURSE HW_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
list(APPEND HW_SRCS 
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_adcConversions.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_pwmInputFreqOnly.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_stackWaterMark.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_QUADRUNA_DIR}")

# Bootloader files.
file(GLOB_RECURSE BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot/" "${CMAKE_CURRENT_SOURCE_DIR}/boot/Inc")

# tests
file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "quadruna_FSM_can"
        "FSM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
        "quadruna"
    )
    embedded_library(
        quadruna_FSM_can
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )
    generate_stm32cube_code(
        "FSM"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        "stm32f4xx_hal_adc_ex.c"
        "stm32f4xx_hal_adc.c"
        "stm32f4xx_hal_cortex.c"
        "stm32f4xx_hal_dma_ex.c"
        "stm32f4xx_hal_dma.c"
        "stm32f4xx_hal_exti.c"
        "stm32f4xx_hal_can.c"
        "stm32f4xx_hal_gpio.c"
        "stm32f4xx_hal_iwdg.c"
        "stm32f4xx_hal_pwr_ex.c"
        "stm32f4xx_hal_rcc_ex.c"
        "stm32f4xx_hal_rcc.c"
        "stm32f4xx_hal_tim_ex.c"
        "stm32f4xx_hal_tim.c"
        # "stm32f4xx_hal_uart_ex.c" # TODO: add back when uart finalized
        "stm32f4xx_hal_uart.c"
        "stm32f4xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32f412rx_cube_library(
        "quadruna_FSM_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${TASKS_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    embedded_binary(
        "quadruna_FSM_app"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )

    commit_info_library(
        "quadruna_FSM_app.elf"
        "quadruna_FSM_commit_info"
        "${CMAKE_CURRENT_BINARY_DIR}/app"
        "${ARM_CORE}"
    )

    target_link_libraries("quadruna_FSM_can" "quadruna_FSM_stm32cube")
    target_link_libraries("quadruna_FSM_app.elf" "quadruna_FSM_stm32cube" "quadruna_FSM_can" "quadruna_FSM_commit_info" "debug_modules" "m")

    # Build bootloader.
    stm32f4_boot_binary(
        "quadruna_FSM_boot"
        "${BOOT_SRCS}"
        "${BOOT_INCLUDE_DIRS}"
        "quadruna_FSM"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/Src/syscalls.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/FSM_boot.ioc"
    )

    embedded_image(
        "quadruna_FSM"
        "quadruna_FSM_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/quadruna_FSM_app.hex"
        "quadruna_FSM_boot.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/quadruna_FSM_boot.hex"
    )
    
elseif("${TARGET}" STREQUAL "test")
    jsoncan_library(
        "quadruna_FSM_can"
        "FSM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
        "quadruna"
    )
    add_library(quadruna_FSM_can STATIC "${CAN_SRCS}")
    target_include_directories(quadruna_FSM_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_steering.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_wheels.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_apps.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_brake.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_suspension.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_loadCell.h"
    )
    create_fake_library(
        "quadruna_FSM_fakes"
        "${HEADERS_TO_FAKE}"
    )
    
    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
        "quadruna_FSM_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("quadruna_FSM_test" "quadruna_FSM_can" "quadruna_FSM_fakes" "shared_fakes" "m")
endif()
