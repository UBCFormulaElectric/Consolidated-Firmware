message("")
message("⚙️ Configuring F4Dev")

set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/f4dev.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app_only.ld")

set(HW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/hw")
set(IO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/io")

file(GLOB_RECURSE EMBEDDED_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Shared sources.
list(APPEND EMBEDDED_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_sd.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_usb.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_adc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_IO_INCLUDE_DIR}/io_chimera_v2.c"
)

list(REMOVE_ITEM EMBEDDED_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/sysmem.c")
set(EMBEDDED_INCLUDE_DIRS 
        "${HW_INCLUDE_DIR}" "${IO_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${SHARED_IO_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_DIR}"
)

IF ("${TARGET}" STREQUAL "binary")
    generate_stm32cube_code(
            "f4dev"
            "${IOC_PATH}"
    )

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
            "stm32f4xx_hal_pcd.c"
            "stm32f4xx_hal_pcd_ex.c"
            "stm32f4xx_ll_usb.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32f412rx_cube_library(
            "f4dev_stm32cube"
            "${STM32_HAL_SRCS}"
            "${SYSCALLS}"
            "${MD5_LOCATION}"
            TRUE
    )

    embedded_binary(
            "f4dev"
            "${EMBEDDED_SRCS}"
            "${EMBEDDED_INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    # Autogenerate git commit info header/source.
    commit_info_library(
            "f4dev.elf"
            "f4dev_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
            "${ARM_CORE}"
    )

    target_link_libraries("f4dev.elf" "f4dev_stm32cube" "f4dev_commit_info" "chimera_v2_proto_cm4")
ENDIF ()
