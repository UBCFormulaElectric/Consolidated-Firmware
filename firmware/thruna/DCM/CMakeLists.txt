set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DCM.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS 
    "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
    "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
    "${SHARED_APP_INCLUDE_DIR}/app_rangeCheck.c"
    "${SHARED_APP_INCLUDE_DIR}/app_heartbeatMonitor.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${CMAKE_CURRENT_SOURCE_DIR}/src/app/vehicle_dynamics" "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_THRUNA_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_IO_INCLUDE_DIR}/io_led.c"
    "${SHARED_IO_INCLUDE_DIR}/io_time.c"
    "${SHARED_IO_INCLUDE_DIR}/io_buzzer.c"
    "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_THRUNA_DIR}")

file(GLOB_RECURSE HW_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
list(APPEND HW_SRCS 
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_stackWaterMark.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_THRUNA_DIR}")

# Bootloader files.
file(GLOB_RECURSE BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot/" "${CMAKE_CURRENT_SOURCE_DIR}/boot/Inc")

file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "thruna_DCM_can"
        "DCM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
        "thruna"
    )
    embedded_library(
        thruna_DCM_can
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )

    generate_stm32cube_code(
        "DCM"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        "stm32f4xx_hal_can.c"
        "stm32f4xx_hal_cortex.c"
        "stm32f4xx_hal_dma_ex.c"
        "stm32f4xx_hal_dma.c"
        "stm32f4xx_hal_exti.c"
        "stm32f4xx_hal_gpio.c"
        "stm32f4xx_hal_iwdg.c"
        "stm32f4xx_hal_rcc_ex.c"
        "stm32f4xx_hal_rcc.c"
        "stm32f4xx_hal_tim_ex.c"
        "stm32f4xx_hal_tim.c"
        "stm32f4xx_hal_uart.c"
        "stm32f4xx_hal.c"
    )
    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32f412rx_cube_library(
        "thruna_DCM_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )
    
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    embedded_binary(
        "thruna_DCM_app"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )

    # Autogenerate git commit info header/source.
    commit_info_library(
        "thruna_DCM_app.elf"
        "thruna_DCM_commit_info"
        "${CMAKE_CURRENT_BINARY_DIR}/app"
        "${ARM_CORE}"
    )

    target_link_libraries("thruna_DCM_can" "thruna_DCM_stm32cube")
    target_link_libraries("thruna_DCM_app.elf" "thruna_DCM_stm32cube" "thruna_DCM_can" "thruna_DCM_commit_info" "sbg_ecom_cm4" m)

    # Build bootloader.
    stm32f4_boot_binary(
        "thruna_DCM_boot"
        "${BOOT_SRCS}"
        "${BOOT_INCLUDE_DIRS}"
        "thruna_DCM"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/Src/syscalls.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/DCM_boot.ioc"
    )

    embedded_image(
        "thruna_DCM"
        "thruna_DCM_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/thruna_DCM_app.hex"
        "thruna_DCM_boot.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/thruna_DCM_boot.hex"
    )

elseif("${TARGET}" STREQUAL "test")
    set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sbgEllipse.h"
    )
    create_fake_library(
        "thruna_DCM_fakes"
        "${HEADERS_TO_FAKE}"
    )
    jsoncan_library(
        "thruna_DCM_can"
        "DCM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
        "thruna"
    )
    add_library(thruna_DCM_can STATIC "${CAN_SRCS}")
    target_include_directories(thruna_DCM_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
        "thruna_DCM_test"
        
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("thruna_DCM_test" "thruna_DCM_can" "thruna_DCM_fakes" "shared_fakes")
endif()
