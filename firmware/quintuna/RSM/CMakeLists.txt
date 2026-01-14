message("")
message("⚙️ Configuring RSM (Quintuna)")

file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")
file(GLOB SYSTEM_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/boot")

# App.
file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
        "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
        "${SHARED_APP_INCLUDE_DIR}/app_rangeCheck.c"
        "${SHARED_APP_INCLUDE_DIR}/app_heartbeatMonitor.c"
        "${SHARED_APP_INCLUDE_DIR}/app_heartbeatBoard.c"
        "${SHARED_APP_INCLUDE_DIR}/app_jsoncan.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

# IO.
file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_imu.c"
        "${SHARED_IO_INCLUDE_DIR}/io_led.c"
        "${SHARED_IO_INCLUDE_DIR}/io_potentiometer.c"
        "${SHARED_IO_INCLUDE_DIR}/io_bootHandler.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

# HW.
file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
)
list(FILTER HW_SRCS EXCLUDE REGEX ".*/hw_chimeraConfig_v2\\.c$")
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_adc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can_f4.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_adc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_pwmInput.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_pwmInputFreqOnly.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_usb.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_resetReason.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_freeRtosConfigs.c"
)
if ("${FIRMWARE_TARGET}" STREQUAL "chimera")
    list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_chimera_v2.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/hw_chimeraConfig_v2.c"
    )
endif()
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${SHARED_HW_INCLUDE_DIR}"
)

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm4")
    if ("${BOOTLOAD}" STREQUAL "ON")
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")
    else ()
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app_only.ld")
    endif ()
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${TASKS_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "quintuna_RSM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    set(STM32_HAL_SRCS
            "stm32f4xx_hal_adc_ex.c"
            "stm32f4xx_hal_adc.c"
            "stm32f4xx_hal_can.c"
            "stm32f4xx_hal_cortex.c"
            "stm32f4xx_hal_dma_ex.c"
            "stm32f4xx_hal_dma.c"
            "stm32f4xx_hal_exti.c"
            "stm32f4xx_hal_gpio.c"
            "stm32f4xx_hal_i2c.c"
            "stm32f4xx_hal_spi.c"
            "stm32f4xx_hal_iwdg.c"
            "stm32f4xx_hal_pcd.c"
            "stm32f4xx_hal_pcd_ex.c"
            "stm32f4xx_hal_pwr_ex.c"
            "stm32f4xx_hal_rcc_ex.c"
            "stm32f4xx_hal_rcc.c"
            "stm32f4xx_hal_tim_ex.c"
            "stm32f4xx_hal_tim.c"
            "stm32f4xx_hal_uart.c"
            "stm32f4xx_hal.c"
            "stm32f4xx_ll_usb.c"
    )
    set(CUBEMX_INCLUDES ${CUBEMX_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})
    stm32f412rx_cube_library(
            "quintuna_RSM_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/RSM.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDES}"
            TRUE
            "${ARM_CORE}"
    )

    # JSONCAN
    jsoncan_embedded_library(RSM quintuna "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "quintuna_RSM_app.elf"
            "quintuna_RSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("quintuna_RSM_app.elf" PRIVATE "quintuna_RSM_stm32" "quintuna_RSM_jsoncan" "quintuna_RSM_commit_info" m)
    if ("${FIRMWARE_TARGET}" STREQUAL "chimera")
        target_link_libraries("quintuna_RSM_app.elf" PRIVATE "chimera_v2_proto_cm4")
    endif()

    if ("${BOOTLOAD}" STREQUAL "ON")
        # Bootloader files.
        file(GLOB BOOT_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
        file(GLOB BOOT_CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
        set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
        no_checks("${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c")
        stm32f4_boot_binary(
                "quintuna_RSM_boot"
                "${BOOT_SRCS}"
                "${BOOT_CUBEMX_SRCS}"
                "${BOOT_INCLUDE_DIRS}"
                "quintuna_RSM"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/RSM_boot.ioc"
        )
        target_link_libraries("quintuna_RSM_boot.elf" PRIVATE "quintuna_RSM_commit_info")

        embedded_image(
                "quintuna_RSM"
                "quintuna_RSM_app.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/quintuna_RSM_app.hex"
                "quintuna_RSM_boot.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/quintuna_RSM_boot.hex"
        )
    endif ()
elseif ("${TARGET}" STREQUAL "test")
endif ()
