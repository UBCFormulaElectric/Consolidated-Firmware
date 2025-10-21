message("")
message("⚙️ Configuring SSM")

# Configs.
set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/ssm.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app_only.ld")

# HW.
set(HW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/hw")
file(GLOB_RECURSE CONFIGURE_DEPENDS HW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c")

# Main.
set(MAIN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")
file(GLOB_RECURSE CONFIGURE_DEPENDS MAIN_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
list(REMOVE_ITEM MAIN_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/sysmem.c")

# Shared HW srcs.
set(HW_SHARED_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_usb.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_spi.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_chimera_v2.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_ubsan.c"
)

# Collect include dirs and srcs.
set(EMBEDDED_INCLUDE_DIRS
        "${HW_INCLUDE_DIR}" "${IO_INCLUDE_DIR}" "${MAIN_INCLUDE_DIR}"
        "${SHARED_IO_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_DIR}"
)
set(EMBEDDED_SRCS "${HW_SRCS}" "${IO_SRCS}" "${HW_SHARED_SRCS}")

# Only binary targets supported for SSM.
IF ("${TARGET}" STREQUAL "binary")
    # Collect HAL.
    set(STM32_HAL_SRCS
            "stm32f4xx_hal_adc_ex.c"
            "stm32f4xx_hal_adc.c"
            "stm32f4xx_hal_can.c"
            "stm32f4xx_hal_cortex.c"
            "stm32f4xx_hal_dma_ex.c"
            "stm32f4xx_hal_dma.c"
            "stm32f4xx_hal_exti.c"
            "stm32f4xx_hal_gpio.c"
            "stm32f4xx_hal_iwdg.c"
            "stm32f4xx_hal_rcc_ex.c"
            "stm32f4xx_hal_rcc.c"
            "stm32f4xx_hal_tim_ex.c"
            "stm32f4xx_hal_tim.c"
            "stm32f4xx_hal_uart.c"
            "stm32f4xx_hal.c"
            "stm32f4xx_hal_sd.c"
            "stm32f4xx_ll_sdmmc.c"
            "stm32f4xx_hal_i2c.c"
            "stm32f4xx_hal_i2c_ex.c"
            "stm32f4xx_hal_pcd.c"
            "stm32f4xx_hal_pcd_ex.c"
            "stm32f4xx_ll_usb.c"
            "stm32f4xx_hal_spi.c"
    )

    # Don't check syscalls.
    no_checks("${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")

    stm32f412rx_cube_library(
            "ssm_stm32"
            "${STM32_HAL_SRCS}"
            "${IOC_PATH}"
            "${MAIN_SRCS}"
            "${MAIN_INCLUDE_DIR}"
            TRUE
            "${ARM_CORE}"
    )

    # Setup binary target.
    embedded_binary(
            "ssm"
            "${EMBEDDED_SRCS}"
            "${EMBEDDED_INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    # Autogenerate git commit info header/source.
    commit_info_library(
            "ssm.elf"
            "ssm_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    # Link.
    target_link_libraries("ssm.elf" PRIVATE "ssm_stm32" "ssm_commit_info" "chimera_v2_proto_cm4")
ENDIF ()
