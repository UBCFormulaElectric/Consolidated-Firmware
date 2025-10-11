message("")
message("⚙️ Configuring H7Dev")

set(ARM_CORE "cm7")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/h7dev.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")

file(GLOB EMBEDDED_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
list(APPEND EMBEDDED_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_canLogging.c"
        "${SHARED_IO_INCLUDE_DIR}/io_fileSystem_logfs.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_sd.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_ubsan.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can_hx.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_usb.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_freeRtosConfigs.c"
)

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")

nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS sample.proto)

set(SRCS ${PROTO_SRCS})
set(HDRS ${CMAKE_CURRENT_BINARY_DIR} ${NANOPB_INCLUDE_DIRS})

set(EMBEDDED_INCLUDE_DIRS
        "${SHARED_APP_INCLUDE_DIR}"
        "${SHARED_IO_INCLUDE_DIR}"
        "${SHARED_HW_INCLUDE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

IF ("${TARGET}" STREQUAL "binary")
    generate_stm32cube_code(
            "h7dev_stm32cube"
            "${IOC_PATH}"
            "${CUBEMX_SRCS}"
    )
    embedded_no_checks("${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")

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
            "stm32h7xx_hal_pcd.c"
            "stm32h7xx_hal_pcd_ex.c"
            "stm32h7xx_ll_usb.c"
            "stm32h7xx_ll_rtc.c"
            "stm32h7xx_hal_rtc.c"
            "stm32h7xx_hal_pwr.c"
            "stm32h7xx_hal_pwr_ex.c"
    )

    stm32h733xx_cube_library(
            "h7dev_stm32cube_hal"
            "${STM32_HAL_SRCS}"
            "${MD5_LOCATION}"
            TRUE
    )

    embedded_binary(
            "h7dev_app"
            "${EMBEDDED_SRCS}"
            "${EMBEDDED_INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    commit_info_library(
            "h7dev_app.elf"
            "h7dev_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
            "${ARM_CORE}"
    )

    embedded_library(
            "protobuf_library"
            "${PROTO_SRCS}"
            "${HDRS}"
            "cm7"
            TRUE
    )

    target_link_libraries("h7dev_app.elf" "h7dev_stm32cube" "h7dev_stm32cube_hal" "protobuf_library" "logfs_cm7")

    # Bootloader files.
    file(GLOB BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    file(GLOB BOOT_CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot/" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
    stm32h7_boot_binary(
            "h7dev_boot"
            "${BOOT_SRCS}"
            "${BOOT_CUBEMX_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "h7dev"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/h7dev_boot.ioc"
    )
    embedded_no_checks("${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c")
    target_link_libraries("h7dev_boot.elf" PRIVATE "h7dev_commit_info")

    embedded_image(
            "h7dev"
            "h7dev_app.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/h7dev_app.hex"
            "h7dev_boot.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/h7dev_boot.hex"
    )
ENDIF ()
