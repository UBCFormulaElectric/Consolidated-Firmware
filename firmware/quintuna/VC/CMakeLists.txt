message("")
message("⚙️ Configuring VC (Quintuna)")

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
        "${SHARED_APP_INCLUDE_DIR}/app_signal.c"
        "${SHARED_APP_INCLUDE_DIR}/app_jsoncan.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_QUINTUNA_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src/app/vehicle_dynamics")


file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_canLogging.c"
        "${SHARED_IO_INCLUDE_DIR}/io_led.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_IO_INCLUDE_DIR}/io_loadswitch.c"
        "${SHARED_IO_INCLUDE_DIR}/io_potentiometer.c"
        "${SHARED_IO_INCLUDE_DIR}/io_bootHandler.c"
        "${SHARED_IO_INCLUDE_DIR}/io_imu.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_QUINTUNA_DIR}")

file(GLOB_RECURSE HW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c")
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_usb.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_adc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can_h7.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_chimera_v2.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_ubsan.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_resetReason.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_freeRtosConfigs.c"
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SHARED_HW_INCLUDE_DIR}")

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm7")
    if ("${BOOTLOAD}" STREQUAL "ON")
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")
    else ()
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app_only.ld")
    endif ()
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})
    embedded_binary("quintuna_VC_app" "${SRCS}" "${INCLUDE_DIRS}" "${LINKER_SCRIPT}" "${ARM_CORE}")

    set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/VC.ioc")
    list(APPEND NO_CHECKS
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/usbd_desc.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/usbd_cdc_if.c"
    )
    embedded_no_checks("${NO_CHECKS}")
    generate_stm32cube_code("quintuna_VC_stm32cube" "${IOC_PATH}" "${CUBEMX_SRCS}")

    # STM HAL Library
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
            "stm32h7xx_hal.c"
            "stm32h7xx_hal_i2c.c"
            "stm32h7xx_hal_i2c_ex.c"
            "stm32h7xx_hal_pcd.c"
            "stm32h7xx_hal_pcd_ex.c"
            "stm32h7xx_ll_usb.c"
    )

    stm32h733xx_cube_library(
            "quintuna_VC_stm32cube_hal"
            "${STM32_HAL_SRCS}"
            "${MD5_LOCATION}"
            TRUE
    )

    # JSONCAN
    jsoncan_embedded_library(VC quintuna "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "quintuna_VC_app.elf"
            "quintuna_VC_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("quintuna_VC_app.elf" PRIVATE
            "quintuna_VC_stm32cube"
            "quintuna_VC_stm32cube_hal"
            "quintuna_VC_jsoncan"
            "quintuna_VC_commit_info"
            "logfs_cm7"
            "sbg_ecom_cm7"
            "chimera_v2_proto_cm7"
            m)


    if ("${BOOTLOAD}" STREQUAL "ON")
        # Bootloader files.
        file(GLOB BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
        file(GLOB BOOT_CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
        set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
        embedded_no_checks("${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c")
        stm32h7_boot_binary(
                "quintuna_VC_boot"
                "${BOOT_SRCS}"
                "${BOOT_CUBEMX_SRCS}"
                "${BOOT_INCLUDE_DIRS}"
                "quintuna_VC"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/VC_boot.ioc"
        )
        target_link_libraries("quintuna_VC_boot.elf" PRIVATE "quintuna_VC_commit_info")

        embedded_image(
                "quintuna_VC"
                "quintuna_VC_app.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/quintuna_VC_app.hex"
                "quintuna_VC_boot.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/quintuna_VC_boot.hex"
        )
    endif ()
elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(FAKE_SRCS
            "${SHARED_FAKES_DIR}/io_imu.cpp"
            "${SHARED_FAKES_DIR}/io_loadswitch.cpp"
            "${SHARED_FAKES_DIR}/io_canQueue.cpp"
            "${SHARED_FAKES_DIR}/io_bootHandler.cpp"
            "${SHARED_FAKES_DIR}/io_time.cpp"
    )
    set(SRCS ${APP_SRCS} ${JOBS_SRC} ${TEST_SRCS} ${FAKE_SRCS})
    set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test"
            ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS}
            ${SYSTEM_INCLUDE_DIRS} ${SHARED_FAKES_DIR}
    )
    compile_gtest_executable(
            "quintuna_VC_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    # JSONCAN
    jsoncan_library(VC quintuna "${CMAKE_CURRENT_BINARY_DIR}")


    # CommitInfo
    commit_info_library(
            "quintuna_VC_test"
            "quintuna_VC_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_compile_definitions("quintuna_VC_test" PRIVATE STM32H733xx)
    target_link_libraries("quintuna_VC_test" PRIVATE "quintuna_VC_jsoncan" "quintuna_VC_commit_info")
endif ()
