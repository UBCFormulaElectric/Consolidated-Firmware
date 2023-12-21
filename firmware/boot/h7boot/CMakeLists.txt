set(ARM_CORE "cm7")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/h7boot.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_boot.ld")

file(GLOB BOOT_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/Src/*.c"
    "${BOOT_DIR}/*.c"
)
list (APPEND BOOT_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_flash.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_fdcan.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
)
set(BOOT_INCLUDE_DIRS 
    "${BOOT_DIR}"
    "${SHARED_IO_INCLUDE_DIR}" 
    "${SHARED_HW_INCLUDE_DIR}" 
)

if("${TARGET}" STREQUAL "deploy")
    generate_stm32cube_code(
        "h7boot"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        "stm32h7xx_hal_adc_ex.c"
        "stm32h7xx_hal_adc.c"
        "stm32h7xx_hal_crc.c"
        "stm32h7xx_hal_crc_ex.c"
        "stm32h7xx_hal_cortex.c"
        "stm32h7xx_hal_dma_ex.c"
        "stm32h7xx_hal_dma.c"
        "stm32h7xx_hal_exti.c"
        "stm32h7xx_hal_fdcan.c"
        "stm32h7xx_hal_flash.c"
        "stm32h7xx_hal_flash_ex.c"
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
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/Src/syscalls.c")
    stm32h733xx_cube_library(
        "h7boot_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    function(stm32h7_bootloader NAME BOARD_DEFINE BOARD_SPECIFIC_SRC)
        set(BOARD_SPECIFIC_SRCS ${BOOT_SRCS} ${BOARD_SPECIFIC_SRC} "${CMAKE_CURRENT_SOURCE_DIR}/bootloader_config_h7.c")
        embedded_binary(
            "${NAME}"
            "${BOARD_SPECIFIC_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
        )
        target_link_libraries("${NAME}.elf" "h7boot_stm32cube")
        target_compile_definitions("${NAME}.elf" PRIVATE "${BOARD_DEFINE}")
    endfunction()

    stm32h7_bootloader("h7dev_boot" "h7dev" "")
endif()
