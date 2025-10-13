message("")
message("⚙️ Configuring H5 Dev")

set(ARM_CORE "cm33")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/h5dev.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h563ritx/stm32h563ritx_app.ld")

file(GLOB EMBEDDED_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
list(APPEND EMBEDDED_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_IO_INCLUDE_DIR}/io_bootHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can_hx.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_freeRtosConfigs.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_ubsan.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_usb.c"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_time.cpp"
        "${SHARED_HW_INCLUDE_DIR}/hw_resetReason.c"
        "${SHARED_APP_INCLUDE_DIR}/app_jsoncan.c"
)

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")

set(EMBEDDED_INCLUDE_DIRS
        "${SHARED_APP_INCLUDE_DIR}"
        "${SHARED_APP_INCLUDE_DIR_CPP}"
        "${SHARED_IO_INCLUDE_DIR}"
        "${SHARED_IO_INCLUDE_DIR_CPP}"
        "${SHARED_HW_INCLUDE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot"
)

IF ("${TARGET}" STREQUAL "binary")
    generate_stm32cube_code(
            "h5dev_stm32cube"
            "${IOC_PATH}"
            "${CUBEMX_SRCS}"
    )
    embedded_no_checks("${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")

    set(STM32_HAL_SRCS
            "stm32h5xx_hal_adc_ex.c"
            "stm32h5xx_hal_adc.c"
            "stm32h5xx_hal_cortex.c"
            "stm32h5xx_hal_dma_ex.c"
            "stm32h5xx_hal_dma.c"
            "stm32h5xx_hal_exti.c"
            "stm32h5xx_hal_fdcan.c"
            "stm32h5xx_hal_gpio.c"
            "stm32h5xx_hal_iwdg.c"
            "stm32h5xx_hal_pwr_ex.c"
            "stm32h5xx_hal_rcc_ex.c"
            "stm32h5xx_hal_rcc.c"
            "stm32h5xx_hal_tim_ex.c"
            "stm32h5xx_hal_tim.c"
            "stm32h5xx_hal_uart_ex.c"
            "stm32h5xx_hal_uart.c"
            "stm32h5xx_ll_sdmmc.c"
            "stm32h5xx_hal_sd_ex.c"
            "stm32h5xx_hal_sd.c"
            "stm32h5xx_hal.c"
            "stm32h5xx_hal_pcd.c"
            "stm32h5xx_hal_pcd_ex.c"
            "stm32h5xx_ll_usb.c"
            "stm32h5xx_ll_rtc.c"
            "stm32h5xx_hal_rtc.c"
            "stm32h5xx_hal_pwr.c"
            "stm32h5xx_hal_pwr_ex.c"
            "stm32h5xx_hal_flash"
    )

    stm32h563xx_cube_library(
            "h5dev_stm32cube_hal"
            "${STM32_HAL_SRCS}"
            "${MD5_LOCATION}"
            TRUE
    )

    # JSONCAN
    jsoncan_embedded_library(H5 dev "${CMAKE_CURRENT_BINARY_DIR}")

    embedded_binary(
            "h5dev_app"
            "${EMBEDDED_SRCS}"
            "${EMBEDDED_INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    ) 

    commit_info_library(
            "h5dev_app.elf"
            "h5dev_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
            "${ARM_CORE}"
    )

    target_link_libraries("h5dev_app.elf" "h5dev_stm32cube" "h5dev_stm32cube_hal" "dev_H5_jsoncan")

    # Bootloader files.
    file(GLOB BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    file(GLOB BOOT_CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot/" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
    stm32h5_boot_binary(
            "h5dev_boot"
            "${BOOT_SRCS}"
            "${BOOT_CUBEMX_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "h5dev"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/h5dev_boot.ioc"
    )
    target_link_libraries("h5dev_boot.elf" PRIVATE "h5dev_commit_info")

    embedded_image(
            "h5dev"
            "h5dev_app.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/h5dev_app.hex"
            "h5dev_boot.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/h5dev_boot.hex"
    )
ENDIF ()
