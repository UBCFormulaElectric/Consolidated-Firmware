IF(NOT "${EMBEDDED_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ embedded.cmake must be included before bootlib.cmake")
ENDIF()
IF(NOT "${STM32LIB_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ stmlib.cmake must be included before bootlib.cmake")
ENDIF()
message("")
message("🥾 Configuring bootloader binary generation")

message("  🔃 Registered stm32f4_boot_binary() function")
function(stm32f4_boot_binary
        BOOT_NAME
        SRCS
        INCLUDE_DIRS
        CONFIG_DEFINE
        SYSCALLS
        IOC_PATH
)
    generate_stm32cube_code(
            "${BOOT_NAME}_cubegen"
            "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
            "stm32f4xx_hal_can.c"
            "stm32f4xx_hal_crc.c"
            "stm32f4xx_hal_cortex.c"
            "stm32f4xx_hal_exti.c"
            "stm32f4xx_hal_flash.c"
            "stm32f4xx_hal_flash_ex.c"
            "stm32f4xx_hal_gpio.c"
            "stm32f4xx_hal_rcc_ex.c"
            "stm32f4xx_hal_rcc.c"
            "stm32f4xx_hal_tim_ex.c"
            "stm32f4xx_hal_tim.c"
            "stm32f4xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    stm32f412rx_cube_library(
            "${BOOT_NAME}_stm32cube"
            "${STM32_HAL_SRCS}"
            "${SYSCALLS}"
            "${MD5_LOCATION}"
            FALSE
    )

    # Add bootloader-specific files.
    list(APPEND SRCS "${BOOT_DIR}/bootloader.c" "${BOOT_DIR}/bootloader_f4.c")
    list(APPEND INCLUDE_DIRS "${BOOT_DIR}")

    # Add shared files.
    list(APPEND SRCS
            "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
            "${SHARED_IO_INCLUDE_DIR}/io_time.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_flash.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_can_f4.c"
    )
    list(APPEND INCLUDE_DIRS
            "${SHARED_APP_INCLUDE_DIR}"
            "${SHARED_IO_INCLUDE_DIR}"
            "${SHARED_HW_INCLUDE_DIR}"
    )

    set(ARM_CORE "cm4")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_boot.ld")
    embedded_binary(
            "${BOOT_NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )
    target_link_libraries("${BOOT_NAME}.elf" PRIVATE "${BOOT_NAME}_stm32cube")
    target_compile_definitions("${BOOT_NAME}.elf" PRIVATE "${CONFIG_DEFINE}")
endfunction()

message("  🔃 Registered stm32h7_boot_binary() function")
function(stm32h7_boot_binary
        BOOT_NAME
        SRCS
        INCLUDE_DIRS
        CONFIG_DEFINE
        SYSCALLS
        IOC_PATH
)
    generate_stm32cube_code(
            "${BOOT_NAME}_cubegen"
            "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
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
            "stm32h7xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    stm32h733xx_cube_library(
            "${BOOT_NAME}_stm32cube"
            "${STM32_HAL_SRCS}"
            "${SYSCALLS}"
            "${MD5_LOCATION}"
            FALSE
    )

    # Add bootloader-specific files.
    list(APPEND SRCS "${BOOT_DIR}/bootloader.c" "${BOOT_DIR}/bootloader_h7.c")
    list(APPEND INCLUDE_DIRS "${BOOT_DIR}")

    # Add shared files.
    list(APPEND SRCS
            "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
            "${SHARED_IO_INCLUDE_DIR}/io_time.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_flash.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_can_h7.c"
    )
    list(APPEND INCLUDE_DIRS
            "${SHARED_APP_INCLUDE_DIR}"
            "${SHARED_IO_INCLUDE_DIR}"
            "${SHARED_HW_INCLUDE_DIR}"
    )

    set(ARM_CORE "cm7")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_boot.ld")
    embedded_binary(
            "${BOOT_NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )
    target_link_libraries("${BOOT_NAME}.elf" PRIVATE "${BOOT_NAME}_stm32cube")
    target_compile_definitions("${BOOT_NAME}.elf" PRIVATE "${CONFIG_DEFINE}")
endfunction()