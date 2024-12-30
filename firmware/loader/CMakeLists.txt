IF ("${TARGET}" STREQUAL "binary")
    message("")
    message("📦 Configuring loader binary generation")

    # Common sources and includes shared by all loader builds
    set(COMMON_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_flash.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
    )

    set(COMMON_INCLUDE_DIRS
        "${SHARED_APP_INCLUDE_DIR}"
        "${SHARED_IO_INCLUDE_DIR}"
        "${SHARED_HW_INCLUDE_DIR}"
    )

    # Function to configure loader binary generation
    function(configure_loader LOADER_NAME BOARD IOC_FILE LINKER_SCRIPT ARM_CORE)
        set(LOADER_DIR "${REPO_ROOT_DIR}/firmware/loader/${BOARD}")
        set(IOC_PATH   "${LOADER_DIR}/cubemx/${IOC_FILE}")

        set(INCLUDE_DIRS
            "${LOADER_DIR}"
            "${LOADER_DIR}/cubemx/Inc"
        )

        generate_stm32cube_code(
            "${LOADER_NAME}_cubegen"
            "${IOC_PATH}"
        )

        set(STM32_HAL_SRCS
            "stm32${BOARD}xx_hal_crc.c"
            "stm32${BOARD}xx_hal_cortex.c"
            "stm32${BOARD}xx_hal_dma.c"
            "stm32${BOARD}xx_hal_exti.c"
            "stm32${BOARD}xx_hal_flash.c"
            "stm32${BOARD}xx_hal_gpio.c"
            "stm32${BOARD}xx_hal_iwdg.c"
            "stm32${BOARD}xx_hal_rcc.c"
            "stm32${BOARD}xx_hal_tim.c"
            "stm32${BOARD}xx_hal.c"
            "stm32${BOARD}xx_hal_pwr_ex.c"
            "stm32${BOARD}xx_hal_sd.c"
            "stm32${BOARD}xx_hal_rcc_ex.c"
            "stm32${BOARD}xx_ll_sdmmc.c"
            "stm32${BOARD}xx_hal_uart.c"
        )

        # Board-specific CAN/FDCAN configuration
        if("${BOARD}" STREQUAL "h7")
            list(APPEND STM32_HAL_SRCS "stm32h7xx_hal_crc_ex.c")
            # set(BOARD_SPECIFIC_CAN_SRC "${SHARED_HW_INCLUDE_DIR}/hw_fdcan.c")

            stm32h733xx_cube_library(
                "${LOADER_NAME}_stm32cube"
                "${LOADER_DIR}/cubemx/Inc"
                "${STM32_HAL_SRCS}"
                "${LOADER_DIR}/cubemx/Src/syscalls.c"
                "${MD5_LOCATION}"
                FALSE
            )

            list(APPEND SRCS
                "${LOADER_DIR}/cubemx/Src/main.c"
                "${LOADER_DIR}/cubemx/Src/system_stm32h7xx.c"
            )

        elseif("${BOARD}" STREQUAL "f4")
            list(APPEND STM32_HAL_SRCS "stm32f4xx_hal_can.c")
            # set(BOARD_SPECIFIC_CAN_SRC "${SHARED_HW_INCLUDE_DIR}/hw_can.c")

            stm32f412rx_cube_library(
                "${LOADER_NAME}_stm32cube"
                "${LOADER_DIR}/cubemx/Inc"
                "${STM32_HAL_SRCS}"
                "${LOADER_DIR}/cubemx/Src/syscalls.c"
                "${MD5_LOCATION}"
                FALSE
            )

            list(APPEND SRCS
                "${LOADER_DIR}/cubemx/Src/main.c"
                "${LOADER_DIR}/cubemx/Src/system_stm32f4xx.c"
            )
        endif()

        list(APPEND SRCS ${COMMON_SRCS}) # maybe add ${BOARD_SPECIFIC_CAN_SRC}
        list(APPEND INCLUDE_DIRS ${COMMON_INCLUDE_DIRS})

        set(ARM_CORE "${ARM_CORE}")
        set(LINKER_SCRIPT "${LINKER_SCRIPT}")

        embedded_binary(
            "${LOADER_NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
        )

        target_link_libraries("${LOADER_NAME}.elf" PRIVATE "${LOADER_NAME}_stm32cube")
        target_compile_definitions("${LOADER_NAME}.elf" PRIVATE "${LOADER_NAME}")
    endfunction()

    # Configure loaders for specific boards
    configure_loader(
        "h7_loader"
        "h7"
        "h7_loader.ioc"
        "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_loader.ld"
        "cm7"
    )

    configure_loader(
        "f4_loader"
        "f4"
        "f4_loader.ioc"
        "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_loader.ld"
        "cm4"
    )
ENDIF ()