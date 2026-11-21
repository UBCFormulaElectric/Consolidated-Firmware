message("")
message("⚙️ Configuring DAM (Seven)")

file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${SHARED_HW_INCLUDE_DIR_CPP}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot"
)

set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.cpp")
set(TASKS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.cpp")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot"
)

# APP layer resources (shared + project specific APP code)
file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.cpp")
set(APP_SHARED_SRCS
        "${SHARED_APP_INCLUDE_DIR_CPP}/app_bootcount.cpp"
        "${SHARED_APP_INCLUDE_DIR_CPP}/app_timer.cpp"
        "${SHARED_APP_INCLUDE_DIR_CPP}/app_crc32.cpp"
        "${SHARED_APP_INCLUDE_DIR_CPP}/app_crc8.cpp"
)
set(APP_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
        "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_DIR_CPP}"
)

# IO layer resources (shared + project specific IO code)
file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
set(IO_SHARED_SRCS
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_time.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_filesystem.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_canLogging.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_shdnLoopNode.cpp"
)
set(IO_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_DIR_CPP}"
)

# TODO: Temporary workaround until seven's CAN JSON is complete. JSONCAN codegen (app_canTx/Rx/Utils/Alerts,
#  io_canTx) is not run for the test target, so these sources are left out of it: they use the generated CAN code
#  directly, or need mocks in test/damMocks.cpp (io::FileSystem, ntpButton, estop nodes), which itself uses it.
#  Delete these lists and the matching lines in the test section once JSONCAN is re-enabled there.
set(APP_SRCS_NEEDING_CAN
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/app_buzzer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/app_heartbeatMonitors.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/app_telemRx.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/app_button.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/app_sd.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/app_damShdnLoop.cpp"
)
set(TEST_SRCS_NEEDING_CAN
        "${CMAKE_CURRENT_SOURCE_DIR}/test/damMocks.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_app_buzzer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_app_telemRx.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_app_sd.cpp"
)

# HW layer resources (shared + project specific HW code)
file(GLOB_RECURSE HW_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.cpp")
set(HW_SHARED_SRCS
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_crc.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_error.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_sd.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_usb.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_can_hx.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_uart.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_hardFaultHandler.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_pwmOutput.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_resetReason.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_bootup.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_runTimeStat.cpp"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_DIR_CPP}"
)

# Chimera sources
set(CHIMERA_SRCS
        ${HW_SRCS}
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_error.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_sd.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_usb.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_can_hx.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_uart.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_hardFaultHandler.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_pwmOutput.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/chimera.cpp"
)
set(CHIMERA_INCLUDE_DIRS ${HW_INCLUDE_DIRS} ${IO_INCLUDE_DIRS})

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm33")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32h562ritx/stm32h562ritx_app.ld")
    set(SRCS ${APP_SRCS} ${APP_SHARED_SRCS} ${IO_SRCS} ${IO_SHARED_SRCS} ${HW_SRCS} ${HW_SHARED_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "seven_DAM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    set(STM32_HAL_SRCS
            "stm32h5xx_hal.c"
            "stm32h5xx_hal_rcc.c"
            "stm32h5xx_hal_rcc_ex.c"
            "stm32h5xx_hal_pwr.c"
            "stm32h5xx_hal_pwr_ex.c"
            "stm32h5xx_hal_cortex.c"
            "stm32h5xx_hal_exti.c"
            "stm32h5xx_hal_tim.c"
            "stm32h5xx_hal_tim_ex.c"
            "stm32h5xx_hal_uart.c"
            "stm32h5xx_hal_uart_ex.c"
            "stm32h5xx_hal_rtc.c"
            "stm32h5xx_hal_gpio.c"
            "stm32h5xx_hal_fdcan.c"
            "stm32h5xx_hal_pcd.c"
            "stm32h5xx_hal_sd.c"
            "stm32h5xx_hal_crc.c"
            "stm32h5xx_ll_sdmmc.c"
            "stm32h5xx_ll_usb.c"
            "stm32h5xx_hal_pcd_ex.c"
            "stm32h5xx_hal_crc_ex.c"
            "stm32h5xx_hal_rtc_ex.c"
            "stm32h5xx_hal_iwdg.c"
            "stm32h5xx_hal_dma.c"
            "stm32h5xx_hal_dma_ex.c"
    )
    set(CUBEMX_INCLUDE_DIRS_ ${CUBEMX_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})
    stm32h562xx_cube_library(
            "seven_DAM_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DAM.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDE_DIRS_}"
            TRUE
            "${ARM_CORE}"
    )

    # JSONCAN
    # TODO: can_bus/seven does not exist yet, so building seven_DAM_app / seven_DAM_chimera fails at codegen
    jsoncan_embedded_library_cpp(DAM seven "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "seven_DAM_app.elf"
            "seven_DAM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("seven_DAM_app.elf" PRIVATE "seven_DAM_stm32" "seven_DAM_commit_info" "seven_DAM_jsoncan" "m" "logfs_cm7")
    target_compile_definitions("seven_DAM_app.elf" PRIVATE DAM)

    # TODO: No seven chimera proto exists yet; reuse hexray's DAM proto (its TSIM net names are simply unused)
    add_chimera_stm32h5("seven_DAM_chimera" "${CHIMERA_SRCS}" "${CHIMERA_INCLUDE_DIRS}" "hexray_chimera_v2_proto_cm33")
    target_link_libraries("seven_DAM_chimera.elf" PRIVATE "seven_DAM_stm32")

    if ("${BOOTLOAD}" STREQUAL "ON")
        target_compile_definitions("seven_DAM_app.elf" PRIVATE BOOTLOAD)

        file(GLOB BOOT_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.cpp")
        file(GLOB BOOT_CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
        set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
        no_checks("${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c")
        stm32h5_boot_binary(
                "seven_DAM_boot"
                "${BOOT_SRCS}"
                "${BOOT_CUBEMX_SRCS}"
                "${BOOT_INCLUDE_DIRS}"
                "seven_DAM"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/DAM_boot.ioc"
        )
        target_link_libraries("seven_DAM_boot.elf" PRIVATE "seven_DAM_commit_info")
        embedded_image(
                "seven_DAM"
                "seven_DAM_app.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/seven_DAM_app.hex"
                "seven_DAM_boot.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/seven_DAM_boot.hex"
        )
    endif ()

elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    # TODO: Workaround, see APP_SRCS_NEEDING_CAN above
    list(REMOVE_ITEM APP_SRCS ${APP_SRCS_NEEDING_CAN})
    list(REMOVE_ITEM TEST_SRCS ${TEST_SRCS_NEEDING_CAN})
    # Only used by app_sd.cpp, and needs the io::FileSystem mock in damMocks.cpp
    list(REMOVE_ITEM APP_SHARED_SRCS "${SHARED_APP_INCLUDE_DIR_CPP}/app_bootcount.cpp")
    set(FAKE_SRCS
            "${SHARED_FAKES_DIR_CPP}/io_time.cpp"
    )
    # TODO: Workaround, restore with JSONCAN (io_canMsg.hpp includes the generated app_canUtils.hpp)
    # set(IO_TEST_SRCS
    #         "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_telemMessage.cpp"
    # )
    set(SRCS ${APP_SRCS} ${APP_SHARED_SRCS} ${TEST_SRCS} ${FAKE_SRCS})
    set(INCLUDE_DIRS
            ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS}
            ${SHARED_FAKES_DIR_CPP} ${SYSTEM_INCLUDE_DIRS}
            ${SHARED_UTIL_INCLUDE_DIR_CPP} ${CUBEMX_INCLUDE_DIRS}
    )
    compile_gtest_executable(
            "seven_DAM_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    add_compile_definitions("seven_DAM_test" PRIVATE STM32H562Rx)

    # TODO: Workaround, re-enable once can_bus/seven is complete
    # jsoncan_library_cpp(DAM seven "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "seven_DAM_test"
            "seven_DAM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("seven_DAM_test" PRIVATE "seven_DAM_commit_info")
    target_compile_definitions("seven_DAM_test" PRIVATE STM32H562xx)
endif ()
