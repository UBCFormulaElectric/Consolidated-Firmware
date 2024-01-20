set(ARM_CORE "cm7")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/h7dev.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")

file(GLOB EMBEDDED_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/Src/*.c")
list (APPEND EMBEDDED_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_IO_INCLUDE_DIR}/io_canLogging.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
    )
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/nanopb/extra)
find_package(Nanopb REQUIRED)
nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS sample.proto)
    
set(SRCS ${PROTO_SRCS})
set(HDRS ${CMAKE_CURRENT_BINARY_DIR} ${NANOPB_INCLUDE_DIRS})
    
set(EMBEDDED_INCLUDE_DIRS "${SHARED_APP_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_DIR}")

if("${TARGET}" STREQUAL "deploy")
    generate_stm32cube_code(
        "h7dev"
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
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/Src/syscalls.c")
    stm32h733xx_cube_library(
        "h7dev_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    embedded_binary(
        "h7dev_app"
        "${EMBEDDED_SRCS}"
        "${EMBEDDED_INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )

    embedded_library(
        "protobuf_library"
        "${SRCS}"
        "${HDRS}"
        "cm7"
        TRUE
    )
    target_link_libraries("h7dev_app.elf" "h7dev_stm32cube" "protobuf_library" "littlefs_cm4")

    embedded_image(
        "h7dev"
        "h7dev_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/h7dev_app.hex"
        "h7dev_boot.hex"
        "${CMAKE_BINARY_DIR}/firmware/boot/h7boot/h7dev_boot.hex"
    )

endif()
