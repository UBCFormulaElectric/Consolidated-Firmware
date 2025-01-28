message("")
message("⚙️ Configuring DAM (Quintuna)")

set(ARM_CORE "cm7")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")

file(GLOB CUBEMX_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
embedded_no_checks("${CUBEMX_SRCS}")
file(GLOB SYSTEM_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
        "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        ""
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
)
list(APPEND HW_SRCS
        ""
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}"
)

if ("${TARGET}" STREQUAL "binary")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${TASKS_SRCS} ${SYSTEM_SRCS} ${CUBEMX_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})

    embedded_binary(
            "quintuna_DAM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    # Bootloader files.
    file(GLOB_RECURSE BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
    #Build Bootloader
    stm32h7_boot_binary(
            "quintuna_DAM_boot"
            "${BOOT_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "quintuna_DAM"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/DAM_boot.ioc"
    )

    set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DAM.ioc")

    generate_stm32cube_code("DAM" "${IOC_PATH}")

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
            "stm32h7xx_hal_pcd.c"
            "stm32h7xx_hal_pcd_ex.c"
    )

    stm32h733xx_cube_library(
            "quintuna_DAM_stm32cube"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
            "${MD5_LOCATION}"
            TRUE
    )


    # JSONCAN
    jsoncan_embedded_library(DAM quintuna "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "quintuna_DAM_app.elf"
            "quintuna_DAM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
            "${ARM_CORE}"
    )

    target_link_libraries("quintuna_DAM_app.elf" PRIVATE "quintuna_DAM_stm32cube" "quintuna_DAM_jsoncan" "quintuna_DAM_commit_info" "debug_modules")
    target_link_libraries("quintuna_DAM_boot.elf" PRIVATE "quintuna_DAM_commit_info")


    #create image
    embedded_image(
            "quintuna_DAM"
            "quintuna_DAM_app.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quintuna_DAM_app.hex"
            "quintuna_DAM_boot.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quintuna_DAM_boot.hex"
    )

elseif ("${TARGET}" STREQUAL "test")
endif ()
