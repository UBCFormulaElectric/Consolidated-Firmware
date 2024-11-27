IF(NOT "${EMBEDDED_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ embedded.cmake must be included before loaderlib.cmake")
ENDIF()
IF(NOT "${STM32LIB_CMAKE_INCLUDED}" STREQUAL "TRUE")
    message(FATAL_ERROR "❌ stmlib.cmake must be included before loaderlib.cmake")
ENDIF()
message("")
message("🥾 Configuring loader binary generation")

message("  🔃 Registered stm32h7_loader_binary() function")

function(stm32f4_loader_binary
        LOADER_NAME
        SRCS
        INCLUDE_DIRS
        CONFIG_DEFINE
        SYSCALLS
        IOC_PATH
)
    generate_stm32cube_code(
            "${LOADER_NAME}_cubegen"
            "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
            "stm32f4xx_hal_can.c"
            "stm32f4xx_hal_crc.c"
            "stm32f4xx_hal_cortex.c"
            "stm32f4xx_hal_exti.c"
            "stm32f4xx_hal_flash.c"
            "stm32f4xx_hal_flash_ex.c"
            "stm32f4xx_hal_rcc_ex.c"
            "stm32f4xx_hal_rcc.c"
            "stm32f4xx_hal_tim_ex.c"
            "stm32f4xx_hal_tim.c"
            "stm32f4xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    stm32f412rx_cube_library(
            "${LOADER_NAME}_stm32cube"
            "${INCLUDE_DIRS}"
            "${STM32_HAL_SRCS}"
            "${SYSCALLS}"
            "${MD5_LOCATION}"
    )

    list(APPEND SRCS
            "${SHARED_IO_INCLUDE_DIR}/io_can.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_flash.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
            "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
    )
    list(APPEND INCLUDE_DIRS
            "${SHARED_APP_INCLUDE_DIR}"
            "${SHARED_IO_INCLUDE_DIR}"
            "${SHARED_HW_INCLUDE_DIR}"
    )

    set(ARM_CORE "cm4")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_loader.ld")
    embedded_binary(
            "${LOADER_NAME}"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )
    target_link_libraries("${LOADER_NAME}.elf" PRIVATE "${LOADER_NAME}_stm32cube")
    target_compile_definitions("${LOADER_NAME}.elf" PRIVATE "${CONFIG_DEFINE}")
endfunction()
