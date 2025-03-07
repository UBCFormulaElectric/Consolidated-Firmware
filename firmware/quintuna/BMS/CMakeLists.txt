message("")
message("⚙️ Configuring [BMS] (Quintuna)")
set(ARM_CORE "cm7")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/BMS.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

file(GLOB SYSTEM_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.cpp")
list(APPEND APP_SRCS
    ""
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR_CPP}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
list(APPEND IO_SRCS
    ""
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR_CPP}")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.cpp"
)
list(APPEND HW_SRCS
    ""
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR_CPP}"
)

if ("${TARGET}" STREQUAL "binary")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS} ${CUBEMX_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})
    embedded_binary("quintuna_BMS_app" "${SRCS}" "${INCLUDE_DIRS}" "${LINKER_SCRIPT}" "${ARM_CORE}")

    # Bootloader files.
    file(GLOB_RECURSE BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")

    # Build bootloader.
    stm32h7_boot_binary(
        "quintuna_BMS_boot"
        "${BOOT_SRCS}"
        "${BOOT_INCLUDE_DIRS}"
        "quintuna_BMS"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/BMS_boot.ioc"
    )

    set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/BMS.ioc")
    generate_stm32cube_code("BMS" "${IOC_PATH}")
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
        "stm32h7xx_hal_i2c.c"
        "stm32h7xx_hal_i2c_ex.c"
    )
    stm32h733xx_cube_library(
        "quintuna_BMS_stm32cube"
        "${STM32_HAL_SRCS}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
        "${MD5_LOCATION}"
        FALSE
    )

    # JSONCAN
    jsoncan_embedded_library(BMS quintuna "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "quintuna_BMS_app.elf"
            "quintuna_BMS_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
            "${ARM_CORE}"
    )

    target_link_libraries("quintuna_BMS_jsoncan" INTERFACE "quintuna_BMS_stm32cube")
    target_link_libraries("quintuna_BMS_app.elf" PRIVATE "quintuna_BMS_stm32cube" "quintuna_BMS_jsoncan" "quintuna_BMS_commit_info" "debug_modules")
    target_link_libraries("quintuna_BMS_boot.elf" PRIVATE "quintuna_BMS_commit_info")

    embedded_image(
        "quintuna_BMS"
        "quintuna_BMS_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/quintuna_BMS_app.hex"
        "quintuna_BMS_boot.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/quintuna_BMS_boot.hex"
    )

elseif ("${TARGET}" STREQUAL "test")
endif ()