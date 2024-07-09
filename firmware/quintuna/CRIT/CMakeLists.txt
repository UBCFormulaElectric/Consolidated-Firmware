message("⚙️ Configuring CRIT")
set(ARM_CORE "cm4")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE SYSTEM_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.cpp")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR_CPP}/app_stateMachine.cpp"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR_CPP}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_led.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_switch.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_shdnLoopNode.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_heartbeatNode.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_log.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_jsoncan.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_canMsgQueue.cpp"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR_CPP}")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.cpp"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_gpio.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_can.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_bootup.cpp"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR_CPP}"
)

if ("${TARGET}" STREQUAL "deploy")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${TASKS_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})
    embedded_binary(
            "quintuna_CRIT_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    # Bootloader files.
    file(GLOB_RECURSE BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot/" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
    # Build bootloader.
    stm32f4_boot_binary(
            "quintuna_CRIT_boot"
            "${BOOT_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "quintuna_CRIT"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/CRIT_boot.ioc"
    )

    set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/CRIT.ioc")
    generate_stm32cube_code("CRIT" "${IOC_PATH}")

    # STM HAL Library
    set(STM32_HAL_SRCS
            "stm32f4xx_hal_adc_ex.c"
            "stm32f4xx_hal_adc.c"
            "stm32f4xx_hal_cortex.c"
            "stm32f4xx_hal_dma_ex.c"
            "stm32f4xx_hal_dma.c"
            "stm32f4xx_hal_exti.c"
            "stm32f4xx_hal_can.c"
            "stm32f4xx_hal_gpio.c"
            "stm32f4xx_hal_iwdg.c"
            "stm32f4xx_hal_pwr_ex.c"
            "stm32f4xx_hal_rcc_ex.c"
            "stm32f4xx_hal_rcc.c"
            "stm32f4xx_hal_tim_ex.c"
            "stm32f4xx_hal_tim.c"
            "stm32f4xx_hal_uart.c"
            "stm32f4xx_hal.c"
    )
    stm32f412rx_cube_library(
            "quintuna_CRIT_stm32cube"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
            "${MD5_LOCATION}"
    )

    # JSONCAN
    jsoncan_embedded_library(CRIT quintuna "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "quintuna_CRIT_app.elf"
            "quintuna_CRIT_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
            "${ARM_CORE}"
    )

    target_link_libraries("quintuna_CRIT_jsoncan" "quintuna_CRIT_stm32cube")
    target_link_libraries("quintuna_CRIT_app.elf" "quintuna_CRIT_stm32cube" "quintuna_CRIT_jsoncan" "quintuna_CRIT_commit_info" "debug_modules")
    target_link_libraries("quintuna_CRIT_boot.elf" "quintuna_CRIT_commit_info")

    embedded_image(
            "quintuna_CRIT"
            "quintuna_CRIT_app.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quintuna_CRIT_app.hex"
            "quintuna_CRIT_boot.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quintuna_CRIT_boot.hex"
    )
elseif ("${TARGET}" STREQUAL "test")
endif ()
message("")