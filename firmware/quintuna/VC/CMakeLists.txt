message("")
message("⚙️ Configuring VC (Quintuna)")
set(ARM_CORE "cm7")
set(IOC_PATH "$CMAKE_CURRENT_SOURCE_DIR/src/cubemx/VC.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")

file(GLOB_RECURSE SYSTEM_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/src/*.c"
)
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_heartbeatMonitor.c"
        "${SHARED_APP_INCLUDE_DIR}/app_heartbeatBoard.c"
        "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
        "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
        "${SHARED_APP_INCLUDE_DIR}/app_shdnLoopNode.c"
        "${SHARED_APP_INCLUDE_DIR}/app_signal.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_QUINTUNA_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_fdcan.c"
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_canLoggingQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_fileSystem_logfs.c"
        "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c"
        "${SHARED_IO_INCLUDE_DIR}/io_chimera.c"
        "${SHARED_IO_INCLUDE_DIR}/io_led.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_IO_INCLUDE_DIR}/io_taskMonitor.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_QUINTUNA_DIR}")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_adc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_sd.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_QUINTUNA_DIR}")

if ("${TARGET}" STREQUAL "binary")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${TASKS_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})
    embedded_binary("quintuna_VC_app" "${SRCS}" "${INCLUDE_DIRS}" "${LINKER_SCRIPT}" "${ARM_CORE}")

    # Bootloader files.
    file(GLOB_RECURSE BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")

    # Build bootloader.
    stm32h7_boot_binary(
        "quintuna_VC_boot" 
        "${BOOT_SRCS}"
        "${BOOT_INCLUDE_DIRS}"
        "quintuna_VC"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/VC_boot.ioc"
    )

    set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/VC.ioc")
    generate_stm32cube_code("VC" "${IOC_PATH}")

    # STM HAL Library
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
        "stm32h7xx_hal_i2c.c"
        "stm32h7xx_hal_i2c_ex.c"
    )

    stm32h733xx_cube_library(
        "quintuna_VC_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
        "${MD5_LOCATION}"
        FALSE
    )

    # JSONCAN
    jsoncan_embedded_library(VC quintuna "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
        "quintuna_VC_app.elf"
        "quintuna_VC_commit_info"
        "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("quintuna_VC_app.elf" PRIVATE 
            "quintuna_VC_stm32cube" 
            "quintuna_VC_jsoncan" 
            "quintuna_VC_commit_info" 
            "logfs_cm7"
            "sbg_ecom_cm7"
            "debug_modules")
    target_link_libraries("quintuna_VC_boot.elf" PRIVATE "quintuna_VC_commit_info")

    embedded_image(
        "quintuna_VC"
        "quintuna_VC_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/quintuna_VC_app.hex"
        "quintuna_VC_boot.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/quintuna_VC_boot.hex"
    )
elseif ("${TARGET}" STREQUAL "test")
endif ()
