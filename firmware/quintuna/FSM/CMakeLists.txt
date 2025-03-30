message("")
message("⚙️ Configuring FSM (Quintuna)")

set(ARM_CORE "cm4")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app_only.ld")

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

file(GLOB SYSTEM_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
        "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_chimera_v2.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_adc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_usb.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}"
)

if ("${TARGET}" STREQUAL "binary")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS} ${CUBEMX_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})
    embedded_no_checks("${CUBEMX_SRCS}")

    embedded_binary(
            "quintuna_FSM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    #bootloader files
    file(GLOB_RECURSE BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
    # Build bootloader
    stm32f4_boot_binary(
            "quintuna_FSM_boot"
            "${BOOT_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "quintuna_FSM"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/FSM_boot.ioc"
    )

    set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/FSM.ioc")
    generate_stm32cube_code("FSM" "${IOC_PATH}")

    set(STM32_HAL_SRCS
            "stm32f4xx_hal.c" #add more hal library here if needed
    )

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
            "stm32f4xx_hal_spi.c"
            "stm32f4xx_hal.c"
            "stm32f4xx_hal_i2c.c"
            "stm32f4xx_hal_pcd.c"
            "stm32f4xx_hal_pcd_ex.c"
            "stm32f4xx_ll_usb.c"
    )

    stm32f412rx_cube_library(
            "quintuna_FSM_stm32cube"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
            "${MD5_LOCATION}"
            TRUE
    )

    # JSONCAN
    jsoncan_embedded_library(FSM quintuna "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "quintuna_FSM_app.elf"
            "quintuna_FSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
            "${ARM_CORE}"
    )

    target_link_libraries("quintuna_FSM_jsoncan" INTERFACE "quintuna_FSM_stm32cube")
    target_link_libraries("quintuna_FSM_app.elf" PRIVATE "quintuna_FSM_stm32cube" "quintuna_FSM_jsoncan" "quintuna_FSM_commit_info" "chimera_v2_proto_cm4")
    target_link_libraries("quintuna_FSM_boot.elf" PRIVATE "quintuna_FSM_commit_info")
    
    embedded_image(
            "quintuna_FSM"
            "quintuna_FSM_app.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quintuna_FSM_app.hex"
            "quintuna_FSM_boot.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quintuna_FSM_boot.hex"
    )
elseif ("${TARGET}" STREQUAL "test")
endif ()