message("")
message("⚙️ Configuring DAM (Quintuna)")

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.c")
set(TASKS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.c")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/boot")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_heartbeatMonitor.c"
        "${SHARED_APP_INCLUDE_DIR}/app_heartbeatBoard.c"
        "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
        "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
        "${SHARED_APP_INCLUDE_DIR}/app_shdnLoopNode.c"
        "${SHARED_APP_INCLUDE_DIR}/app_jsoncan.c"
        "${SHARED_APP_INCLUDE_DIR}/app_signal.c"
        "${SHARED_APP_INCLUDE_DIR}/app_jsoncan.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_canLogging.c"
        "${SHARED_IO_INCLUDE_DIR}/io_fileSystem_logfs.c"
        # "${SHARED_IO_INCLUDE_DIR}/io_chimera.c"
        # "${SHARED_IO_INCLUDE_DIR}/io_led.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_IO_INCLUDE_DIR}/io_bootHandler.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_sd.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can_h7.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_usb.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_chimera_v2.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_ubsan.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_resetReason.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_freeRtosConfigs.c"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}"
)

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm7")
    if ("${BOOTLOAD}" STREQUAL "ON") # TODO: bootloader does not work on dam yet.
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")
    else ()
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app_only.ld")
    endif ()
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "quintuna_DAM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DAM.ioc")
    list(APPEND NO_CHECKS
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/usbd_desc.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/usbd_cdc_if.c"
    )
    embedded_no_checks("${NO_CHECKS}")
    generate_stm32cube_code("quintuna_DAM_stm32cube" "${IOC_PATH}" "${CUBEMX_SRCS}")

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

    stm32h733xx_cube_library(
            "quintuna_DAM_stm32cube_hal"
            "${STM32_HAL_SRCS}"
            "${MD5_LOCATION}"
            TRUE
    )

    # Generate the protobuf files.
    nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS proto/telem.proto)
    set(P_SRCS ${PROTO_SRCS})
    set(P_HDRS ${CMAKE_CURRENT_BINARY_DIR} ${NANOPB_INCLUDE_DIRS})
    embedded_library(
            "quintuna_DAM_proto"
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
            "${ARM_CORE}"
    )

    target_link_libraries("quintuna_DAM_app.elf" PRIVATE "quintuna_DAM_stm32cube" "quintuna_DAM_stm32cube_hal" "quintuna_DAM_jsoncan" "quintuna_DAM_commit_info" "chimera_v2_proto_cm7" "quintuna_DAM_proto" "logfs_cm7")

    message("Bootloader: ${BOOTLOAD}")
    # if ("${BOOTLOAD}" STREQUAL "ON")
    # Bootloader files.
    file(GLOB BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    file(GLOB BOOT_CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
    embedded_no_checks("${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c")
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
        file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
        set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test" "${SHARED_TEST_UTILS_INCLUDE_DIRS}")
        set(FAKE_SRCS
            "${SHARED_FAKES_DIR}/io_canQueue.cpp"
            "${SHARED_FAKES_DIR}/io_bootHandler.cpp"
        )
        set(SRCS ${APP_SRCS} ${TEST_SRCS} ${FAKE_SRCS})
        set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${SHARED_FAKES_DIR})
        compile_gtest_executable(
                "quintuna_DAM_test"
                "${SRCS}"
                "${INCLUDE_DIRS}"
        )

        # JSONCAN
        jsoncan_library(DAM quintuna "${CMAKE_CURRENT_BINARY_DIR}")
        
        commit_info_library(
            "quintuna_DAM_test"
            "quintuna_DAM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
        )

        #proto 
        nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS proto/telem.proto)
        set(P_SRCS ${PROTO_SRCS})
        set(P_HDRS ${CMAKE_CURRENT_BINARY_DIR} ${NANOPB_INCLUDE_DIRS})
        add_library(quintuna_DAM_proto_test STATIC ${P_SRCS})
        target_include_directories(quintuna_DAM_proto_test PUBLIC ${P_HDRS})
        # FAKE IO
        set(HEADERS_TO_FAKE
        "${SHARED_IO_INCLUDE_DIR}/io_log.h"
        "${SHARED_IO_INCLUDE_DIR}/io_time.h"
        )
        create_fake_library(
                "quintuna_DAM_fakes"
                "${HEADERS_TO_FAKE}"
        )
        target_compile_definitions("quintuna_DAM_test" PRIVATE STM32H733xx)
        target_link_libraries("quintuna_DAM_test" PRIVATE "quintuna_DAM_jsoncan" "quintuna_DAM_fakes" "quintuna_DAM_proto_test")
        target_compile_definitions("quintuna_DAM_test" PRIVATE STM32H733xx)
endif ()
