IF (NOT "${EMBEDDED_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ embedded.cmake must be included before bootlib.cmake")
ENDIF ()
IF (NOT "${STM32LIB_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ stmlib.cmake must be included before bootlib.cmake")
ENDIF ()
message("")
message("🥾 Configuring bootloader binary generation")

message("  🔃 Registered stm32f4_boot_binary() function")
function(stm32f4_boot_binary
        BOOT_NAME
        SRCS
        CUBEMX_SRCS
        INCLUDE_DIRS
        CONFIG_DEFINE
        IOC_PATH
)
    # Add bootloader-specific files.
    list(APPEND SRCS "${BOOT_DIR}/bootloader.c" "${BOOT_DIR}/bootloader_f4.c")
    list(APPEND INCLUDE_DIRS "${BOOT_DIR}")

    # Add shared files.
    list(APPEND SRCS
            "${SHARED_APP_INCLUDE_DIR}/crc32.c"
            "${SHARED_IO_INCLUDE_DIR}/canQueue.c"
            "${SHARED_HW_INCLUDE_DIR}/flash.c"
            "${SHARED_IO_INCLUDE_DIR}/time.c"
            "${SHARED_HW_INCLUDE_DIR}/gpio.c"
            "${SHARED_HW_INCLUDE_DIR}/hardFaultHandler.c"
            "${SHARED_HW_INCLUDE_DIR}/bootup.c"
            "${SHARED_HW_INCLUDE_DIR}/assert.c"
            "${SHARED_HW_INCLUDE_DIR}/error.c"
            "${SHARED_HW_INCLUDE_DIR}/ubsan.c"
            "${SHARED_HW_INCLUDE_DIR}/can_f4.c"
            "${SHARED_HW_INCLUDE_DIR}/utils.c"
            "${SHARED_HW_INCLUDE_DIR}/freeRtosConfigs.c"
    )
    list(APPEND INCLUDE_DIRS
            "${SHARED_EMBEDDED_DIR}"
            "${SHARED_EMBEDDED_DIR_CPP}"
    )


    set(STM32_HAL_SRCS
            "stm32f4xx_hal_can.c"
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
    stm32f412rx_cube_library(
            "${BOOT_NAME}_stm32"
            "${STM32_HAL_SRCS}"
            "${IOC_PATH}"
            "${CUBEMX_SRCS}"
            "${INCLUDE_DIRS}"
            FALSE
            "${ARM_CORE}"
    )

    embedded_binary(
            "${BOOT_NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_boot.ld"
            "cm4"
    )
    target_link_libraries("${BOOT_NAME}.elf" PRIVATE "${BOOT_NAME}_stm32")
    target_compile_definitions("${BOOT_NAME}.elf" PRIVATE "${CONFIG_DEFINE}")
endfunction()

message("  🔃 Registered stm32h7_boot_binary() function")
function(stm32h7_boot_binary
        BOOT_NAME
        SRCS
        CUBEMX_SRCS
        INCLUDE_DIRS
        CONFIG_DEFINE
        IOC_PATH
)
    # Add bootloader-specific files.
    list(APPEND SRCS "${BOOT_DIR}/bootloader.c" "${BOOT_DIR}/bootloader_h7.c")
    list(APPEND INCLUDE_DIRS "${BOOT_DIR}")

    # Add shared files.
    list(APPEND SRCS
            "${SHARED_APP_INCLUDE_DIR}/crc32.c"
            "${SHARED_IO_INCLUDE_DIR}/canQueue.c"
            "${SHARED_IO_INCLUDE_DIR}/time.c"
            "${SHARED_HW_INCLUDE_DIR}/flash.c"
            "${SHARED_HW_INCLUDE_DIR}/gpio.c"
            "${SHARED_HW_INCLUDE_DIR}/hardFaultHandler.c"
            "${SHARED_HW_INCLUDE_DIR}/bootup.c"
            "${SHARED_HW_INCLUDE_DIR}/assert.c"
            "${SHARED_HW_INCLUDE_DIR}/error.c"
            "${SHARED_HW_INCLUDE_DIR}/can_h7.c"
            "${SHARED_HW_INCLUDE_DIR}/ubsan.c"
            "${SHARED_HW_INCLUDE_DIR}/utils.c"
    )
    list(APPEND INCLUDE_DIRS
            "${SHARED_EMBEDDED_DIR}"
            "${SHARED_EMBEDDED_DIR_CPP}"
    )

    set(STM32_HAL_SRCS
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
    stm32h733xx_cube_library(
            "${BOOT_NAME}_stm32"
            "${STM32_HAL_SRCS}"
            "${IOC_PATH}"
            "${CUBEMX_SRCS}"
            "${INCLUDE_DIRS}"
            FALSE
            "${ARM_CORE}"
    )

    embedded_binary(
            "${BOOT_NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_boot.ld"
            "cm7"
    )
    target_link_libraries("${BOOT_NAME}.elf" PRIVATE "${BOOT_NAME}_stm32")
    target_compile_definitions("${BOOT_NAME}.elf" PRIVATE "${CONFIG_DEFINE}")
endfunction()
