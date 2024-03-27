set(ARM_CORE "cm7")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/VC.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS 
    "${SHARED_APP_INCLUDE_DIR}/app_heartbeatMonitor.c"
    "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
    "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${CMAKE_CURRENT_SOURCE_DIR}/src/app/vehicle_dynamics" "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_QUADRUNA_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c"
    "${SHARED_IO_INCLUDE_DIR}/io_chimera.c"
    "${SHARED_IO_INCLUDE_DIR}/io_led.c"
    "${SHARED_IO_INCLUDE_DIR}/io_buzzer.c"
    "${SHARED_IO_INCLUDE_DIR}/io_time.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_QUADRUNA_DIR}")

file(GLOB_RECURSE HW_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
list(APPEND HW_SRCS 
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_fdcan.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_stackWaterMark.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_adcConversions.c"
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_QUADRUNA_DIR}")

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "quadruna_VC_can"
        "VC"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
        "quadruna"
    )
    embedded_library(
        quadruna_VC_can
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
        "stm32h7xx_ll_sdmmc.c"
        "stm32h7xx_hal_sd_ex.c"
        "stm32h7xx_hal_sd.c"
        "stm32h7xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32h733xx_cube_library(
        "quadruna_VC_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${TASKS_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    embedded_binary(
        "quadruna_VC_app"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )

    # Autogenerate git commit info header/source.
    commit_info_library(
        "quadruna_VC_app.elf"
        "quadruna_VC_commit_info"
        "${CMAKE_CURRENT_BINARY_DIR}/app"
        "${ARM_CORE}"
    )

    target_link_libraries("quadruna_VC_can" "quadruna_VC_stm32cube" "sbg_ecom_cm4")
    target_link_libraries("quadruna_VC_app.elf" "quadruna_VC_stm32cube" "quadruna_VC_can" "quadruna_VC_commit_info" "debug_modules" "m")

    embedded_image(
        "quadruna_VC"
        "quadruna_VC_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/quadruna_VC_app.hex"
        "quadruna_VC_boot.hex"
        "${CMAKE_BINARY_DIR}/firmware/boot/h7boot/quadruna_VC_boot.hex"
    )

elseif("${TARGET}" STREQUAL "test")
    jsoncan_library(
        "quadruna_VC_can"
        "VC"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
        "quadruna"
    )


    set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sbgEllipse.h"
        )   

    add_library(quadruna_VC_can STATIC "${CAN_SRCS}")
    target_include_directories(quadruna_VC_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_lowVoltageBattery.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_efuse.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_shutdown.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_currentSensing.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sbgEllipse.h"
    )

    create_fake_library(
        "quadruna_VC_fakes"
        "${HEADERS_TO_FAKE}"
    )
    
    file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
        "quadruna_VC_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("quadruna_VC_test" "quadruna_VC_can" "quadruna_VC_fakes" "shared_fakes")
endif()
