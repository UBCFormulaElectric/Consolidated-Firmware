message("")
message("⚙️ Configuring RSM (Quintuna)")

set(ARM_CORE "cm4")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE SYSTEM_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" # Revisit.
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

file(GLOB SYSTEM_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# App.
file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        ""
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

# IO.
file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        ""
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

# HW.
file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_adc.c"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}"
)

if("${TARGET}" STREQUAL "binary")
        set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${TASKS_SRCS} ${SYSTEM_SRCS})
        embedded_no_checks("${CUBEMX_SRCS}")
        set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

        embedded_binary(
                "quintuna_RSM_app"
                "${SRCS}"
                "${INCLUDE_DIRS}"
                "${LINKER_SCRIPT}"
                "${ARM_CORE}"
        )

        # Bootloader files.
        file(GLOB_RECURSE BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
        set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")

        stm32f4_boot_binary(
                "quintuna_RSM_boot"
                "${BOOT_SRCS}"
                "${BOOT_INCLUDE_DIRS}"
                "quintuna_RSM"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/RSM_boot.ioc"
        )

        set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/RSM.ioc")
        generate_stm32cube_code("RSM" "${IOC_PATH}")

        set(STM32_HAL_SRCS
                "stm32f4xx_hal_adc_ex.c"
                "stm32f4xx_hal_adc.c"
                "stm32f4xx_hal_can.c"
                "stm32f4xx_hal_cortex.c"
                "stm32f4xx_hal_dma_ex.c"
                "stm32f4xx_hal_dma.c"
                "stm32f4xx_hal_exti.c"
                "stm32f4xx_hal_gpio.c"
                "stm32f4xx_hal_i2c.c"
                "stm32f4xx_hal_iwdg.c"
                "stm32f4xx_hal_pcd.c"
                "stm32f4xx_hal_pcd_ex.c"
                "stm32f4xx_hal_pwr_ex.c"
                "stm32f4xx_hal_rcc_ex.c"
                "stm32f4xx_hal_rcc.c"
                "stm32f4xx_hal_tim_ex.c"
                "stm32f4xx_hal_tim.c"
                "stm32f4xx_hal_uart.c"
                "stm32f4xx_hal.c"
                "stm32f4xx_ll_usb.c"
        )

        stm32f412rx_cube_library(
                "quintuna_RSM_stm32cube"
                "${STM32_HAL_SRCS}"
                "${CMAKE_CURRENT_SORUCE_DIR}"
                "${MD5_LOCATION}"
                TRUE
        )

        # JSONCAN
        jsoncan_embedded_library(RSM quintuna "${CMAKE_CURRENT_BINARY_DIR}")

        # CommitInfo
        commit_info_library(
                "quintuna_RSM_app.elf"
                "quintuna_RSM_commit_info"
                "${CMAKE_CURRENT_BINARY_DIR}/app"
        )

        target_link_libraries("quintuna_RSM_app.elf" PRIVATE "quintuna_RSM_stm32cube" "quintuna_RSM_jsoncan" "quintuna_RSM_commit_info" "debug_modules")
        target_link_libraries("quintuna_RSM_boot.elf" PRIVATE "quintuna_RSM_commit_info")

        embedded_image(
                "quintuna_RSM"
                "quintuna_RSM_app.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/quintuna_RSM_app.hex"
                "quintuna_RSM_boot.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/quintuna_RSM_boot.hex"
        )
elseif("${TARGET}" STREQUAL "test")
endif()