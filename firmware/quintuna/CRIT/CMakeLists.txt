message("")
message("⚙️ Configuring CRIT (Quintuna)")
set(ARM_CORE "cm4")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app_only.ld")

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

file(GLOB SYSTEM_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_heartbeatMonitor.c"
        "${SHARED_APP_INCLUDE_DIR}/app_heartbeatBoard.c"
        "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_IO_INCLUDE_DIR}/io_chimera_v2.c"
        "${SHARED_IO_INCLUDE_DIR}/io_timer.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_pwmOutput.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_usb.c"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}"
)

if ("${TARGET}" STREQUAL "binary")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS} ${CUBEMX_SRCS})
    embedded_no_checks("${CUBEMX_SRCS}")
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})
    embedded_binary(
            "quintuna_CRIT_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    # Bootloader files.
    file(GLOB_RECURSE BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot/" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
    # Build bootloader.
    stm32f4_boot_binary(
            "quintuna_CRIT_boot"
            "${BOOT_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "quintuna_CRIT"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/CRIT_boot.ioc"
    )

    set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/CRIT.ioc")
    generate_stm32cube_code("CRIT" "${IOC_PATH}")

    # STM HAL Library
    set(STM32_HAL_SRCS
            "stm32f4xx_hal.c"
            "stm32f4xx_hal_adc.c"
            "stm32f4xx_hal_adc_ex.c"
            "stm32f4xx_hal_can.c"
            "stm32f4xx_hal_cortex.c"
            "stm32f4xx_hal_dma.c"
            "stm32f4xx_hal_dma_ex.c"
            "stm32f4xx_hal_exti.c"
            "stm32f4xx_hal_gpio.c"
            "stm32f4xx_hal_iwdg.c"
            "stm32f4xx_hal_pcd.c"
            "stm32f4xx_hal_pcd_ex.c"
            "stm32f4xx_hal_pwr_ex.c"
            "stm32f4xx_hal_rcc.c"
            "stm32f4xx_hal_rcc_ex.c"
            "stm32f4xx_hal_spi.c"
            "stm32f4xx_hal_tim.c"
            "stm32f4xx_hal_tim_ex.c"
            "stm32f4xx_hal_uart.c"
            "stm32f4xx_hal_flash.c"
            "stm32f4xx_hal_flash_ex.c"
            "stm32f4xx_hal_pwr.c"
            "stm32f4xx_ll_usb.c"
            "stm32f4xx_hal_i2c.c"
            "stm32f4xx_hal_i2c_ex.c"
    )
    stm32f412rx_cube_library(
            "quintuna_CRIT_stm32cube"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
            "${MD5_LOCATION}"
            TRUE
    )

    # JSONCAN
    jsoncan_embedded_library(CRIT quintuna "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "quintuna_CRIT_app.elf"
            "quintuna_CRIT_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("quintuna_CRIT_app.elf" PRIVATE "quintuna_CRIT_stm32cube" "quintuna_CRIT_jsoncan" "quintuna_CRIT_commit_info" "chimera_v2_proto_cm4")
    target_link_libraries("quintuna_CRIT_boot.elf" PRIVATE "quintuna_CRIT_commit_info")

    embedded_image(
            "quintuna_CRIT"
            "quintuna_CRIT_app.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quintuna_CRIT_app.hex"
            "quintuna_CRIT_boot.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quintuna_CRIT_boot.hex"
    )
elseif ("${TARGET}" STREQUAL "test")
endif ()