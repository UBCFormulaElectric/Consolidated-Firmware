message("")
message("⚙️ Configuring DAM (Quintuna)")

file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

file(GLOB SYSTEM_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/boot")

file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/heartbeatMonitor.c"
        "${SHARED_APP_INCLUDE_DIR}/heartbeatBoard.c"
        "${SHARED_APP_INCLUDE_DIR}/stateMachine.c"
        "${SHARED_APP_INCLUDE_DIR}/timer.c"
        "${SHARED_APP_INCLUDE_DIR}/shdnLoopNode.c"
        "${SHARED_APP_INCLUDE_DIR}/jsoncan.c"
        "${SHARED_APP_INCLUDE_DIR}/signal.c"
        "${SHARED_APP_INCLUDE_DIR}/jsoncan.c"
)
set(APP_INCLUDE_DIRS "")

file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/canLogging.c"
        "${SHARED_IO_INCLUDE_DIR}/fileSystem_logfs.c"
        # "${SHARED_IO_INCLUDE_DIR}/chimera.c"
        # "${SHARED_IO_INCLUDE_DIR}/led.c"
        "${SHARED_IO_INCLUDE_DIR}/time.c"
        "${SHARED_IO_INCLUDE_DIR}/bootHandler.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/watchdog.c"
        "${SHARED_HW_INCLUDE_DIR}/gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/assert.c"
        "${SHARED_HW_INCLUDE_DIR}/bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/uart.c"
        "${SHARED_HW_INCLUDE_DIR}/error.c"
        "${SHARED_HW_INCLUDE_DIR}/sd.c"
        "${SHARED_HW_INCLUDE_DIR}/can_h7.c"
        "${SHARED_HW_INCLUDE_DIR}/usb.c"
        "${SHARED_HW_INCLUDE_DIR}/i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/chimera_v2.c"
        "${SHARED_HW_INCLUDE_DIR}/crc.c"
        "${SHARED_HW_INCLUDE_DIR}/utils.c"
        "${SHARED_HW_INCLUDE_DIR}/ubsan.c"
        "${SHARED_HW_INCLUDE_DIR}/resetReason.c"
        "${SHARED_HW_INCLUDE_DIR}/bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/freeRtosConfigs.c"
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw")

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm7")
    if ("${BOOTLOAD}" STREQUAL "ON") # TODO: bootloader does not work on dam yet.
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")
    else ()
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app_only.ld")
    endif ()
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_EMBEDDED_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "quintuna_DAM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    set(STM32_HAL_SRCS
            "stm32h7xx_hal.c"
            "stm32h7xx_hal_crc.c"
            "stm32h7xx_hal_crc_ex.c"
            "stm32h7xx_hal_adc.c"
            "stm32h7xx_hal_adc_ex.c"
            "stm32h7xx_hal_cortex.c"
            "stm32h7xx_hal_dma.c"
            "stm32h7xx_hal_dma_ex.c"
            "stm32h7xx_hal_exti.c"
            "stm32h7xx_hal_fdcan.c"
            "stm32h7xx_hal_gpio.c"
            "stm32h7xx_hal_i2c.c"
            "stm32h7xx_hal_spi.c"
            "stm32h7xx_hal_i2c_ex.c"
            "stm32h7xx_hal_iwdg.c"
            "stm32h7xx_hal_mmc.c"
            "stm32h7xx_hal_pcd.c"
            "stm32h7xx_hal_pcd_ex.c"
            "stm32h7xx_hal_pwr_ex.c"
            "stm32h7xx_hal_rcc.c"
            "stm32h7xx_hal_rcc_ex.c"
            "stm32h7xx_hal_rtc.c"
            "stm32h7xx_hal_sd.c"
            "stm32h7xx_hal_sd_ex.c"
            "stm32h7xx_hal_tim.c"
            "stm32h7xx_hal_tim_ex.c"
            "stm32h7xx_hal_uart.c"
            "stm32h7xx_hal_uart_ex.c"
            "stm32h7xx_ll_sdmmc.c"
            "stm32h7xx_ll_usb.c"
    )
    set(CUBEMX_INCLUDES ${CUBEMX_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})
    stm32h733xx_cube_library(
            "quintuna_DAM_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DAM.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDES}"
            TRUE
            "${ARM_CORE}"
    )

    # Generate the protobuf files.
    nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS proto/telem.proto)
    set(P_SRCS ${PROTO_SRCS})
    set(P_HDRS ${CMAKE_CURRENT_BINARY_DIR} ${NANOPB_INCLUDE_DIRS})
    embedded_library(
            "telem_proto1"
            "${P_SRCS}"
            "${P_HDRS}"
            "cm7"
            TRUE
    )

    # JSONCAN
    jsoncan_embedded_library(DAM quintuna "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "quintuna_DAM_app.elf"
            "quintuna_DAM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("quintuna_DAM_app.elf" PRIVATE "quintuna_DAM_stm32" "quintuna_DAM_jsoncan" "quintuna_DAM_commit_info" "chimera_v2_proto_cm7" "telem_proto1" "logfs_cm7")

    # if ("${BOOTLOAD}" STREQUAL "ON") TODO configure bootloading optionality for DAM
    # Bootloader files.
    file(GLOB BOOT_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    file(GLOB BOOT_CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
    no_checks("${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c")
    stm32h7_boot_binary(
            "quintuna_DAM_boot"
            "${BOOT_SRCS}"
            "${BOOT_CUBEMX_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "quintuna_DAM"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/DAM_boot.ioc"
    )
    target_link_libraries("quintuna_DAM_boot.elf" PRIVATE "quintuna_DAM_commit_info")

    #create image
    embedded_image(
            "quintuna_DAM"
            "quintuna_DAM_app.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quintuna_DAM_app.hex"
            "quintuna_DAM_boot.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quintuna_DAM_boot.hex"
    )
    # endif ()
elseif ("${TARGET}" STREQUAL "test")
endif ()
