message("")
message("⚙️ Configuring RSM (Seven)")

# Cmake Configuration to glob cubemx source code
# TODO: Add project specific and shared sources used in project
file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "")
set(CUBEMX_INCLUDE_DIRS "")

# Task and Job level source file glob
# TODO: Add project specific and shared sources used in project
set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.cpp")
set(TASKS_SRC "")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot"
)

# APP layer resources (shared + project specific APP code)
# TODO: Add project specific and shared sources used in project
file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.cpp")
set(APP_SHARED_SRCS
        "${SHARED_APP_INCLUDE_DIR_CPP}/app_timer.cpp"
)
set(APP_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
        "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_DIR_CPP}"
)

# IO layer resources (shared + project specific IO code)
# TODO: Add project specific and shared sources used in project
file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
set(IO_SHARED_SRCS
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_time.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_imu.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_shdnLoopNode.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_pump.cpp"
)
set(IO_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_DIR_CPP}"
)

# IO layer resources (shared + project specific HW code)
# TODO: Add project specific and shared sources used in project
file(GLOB_RECURSE HW_SRCS CONFIGURE_DEPENDS "")
set(HW_SHARED_SRCS
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_i2c.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_can_hx.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_spi.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_error.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_usb.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_hardFaultHandler.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_pwmOutput.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_bootup.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_resetReason.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_runTimeStat.cpp"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_DIR_CPP}"
)

# Chimera sources
# TODO: Add project specific and shared sources used in project
set(CHIMERA_SRCS
)
set(CHIMERA_INCLUDE_DIRS
)

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm7")

    if ("${BOOTLOAD}" STREQUAL "ON")
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h562vgtx/stm32h562vgtx_app.ld")
    else ()
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h562vgtx/stm32h562vgtx_app_only.ld")
    endif ()
    set(SRCS ${APP_SRCS} ${APP_SHARED_SRCS} ${IO_SRCS} ${IO_SHARED_SRCS} ${HW_SRCS} ${HW_SHARED_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "seven_RSM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    #TODO: Add necessary HAL source if require new driver
    set(STM32_HAL_SRCS
            "stm32h5xx_hal.c"
            "stm32h5xx_hal_rcc.c"
            "stm32h5xx_hal_rcc_ex.c"
            "stm32h5xx_hal_pwr_ex.c"
            "stm32h5xx_hal_cortex.c"
            "stm32h5xx_hal_exti.c"
            "stm32h5xx_hal_tim.c"
            "stm32h5xx_hal_gpio.c"
            "stm32h5xx_hal_adc.c"
            "stm32h5xx_hal_dma.c"
            "stm32h5xx_hal_iwdg.c"

            "stm32h5xx_hal_adc_ex.c"
            "stm32h5xx_hal_tim_ex.c"
            "stm32h5xx_hal_dma_ex.c"

            "stm32h5xx_hal_crc.c"
            "stm32h5xx_hal_crc_ex.c"
            "stm32h5xx_hal_fdcan.c"
            "stm32h5xx_hal_spi.c"
            "stm32h5xx_hal_spi_ex.c"

            "stm32h5xx_hal_sd.c"
            "stm32h5xx_hal_sd_ex.c"
            "stm32h5xx_ll_sdmmc.c"

            "stm32h5xx_hal_pcd.c"
            "stm32h5xx_hal_pcd_ex.c"
            "stm32h5xx_ll_usb.c"

            "stm32h5xx_hal_i2c.c"
    )

    set(CUBEMX_INCLUDES ${CUBEMX_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    stm32h562xx_cube_library(
            "seven_RSM_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/RSM.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDES}"
            TRUE
            "${ARM_CORE}"
    )

    # JSONCAN
    jsoncan_embedded_library_cpp(RSM seven "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})


    # CommitInfo
    commit_info_library(
            "seven_RSM_app.elf"
            "seven_RSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("seven_RSM_app.elf" PRIVATE "seven_RSM_stm32" "seven_RSM_jsoncan" "seven_RSM_commit_info" "m")
    target_compile_definitions("seven_RSM_app.elf" PRIVATE RSM)

    add_chimera_stm32h7("seven_RSM_chimera" "${CHIMERA_SRCS}" "${CHIMERA_INCLUDE_DIRS}" "seven_chimera_v2_proto_cm7")
    target_link_libraries("seven_RSM_chimera.elf" PRIVATE "seven_RSM_stm32")

    if ("${BOOTLOAD}" STREQUAL "ON")
        target_compile_definitions("seven_RSM_app.elf" PRIVATE BOOTLOAD)
        # TODO: Add project specific and shared sources used in project for bootloader

        file(GLOB BOOT_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.cpp")
        file(GLOB BOOT_CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
        set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
        no_checks("")
        stm32h7_boot_binary(
                "seven_RSM_boot"
                "${BOOT_SRCS}"
                "${BOOT_CUBEMX_SRCS}"
                "${BOOT_INCLUDE_DIRS}"
                "seven_RSM"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/RSM_boot.ioc"
        )
        target_link_libraries("seven_RSM_boot.elf" PRIVATE "seven_RSM_commit_info")
        embedded_image(
                "seven_RSM"
                "seven_RSM_app.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/seven_RSM_app.hex"
                "seven_RSM_boot.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/seven_RSM_boot.hex"
        )
    endif ()

elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(FAKE_SRCS
            "${SHARED_FAKES_DIR_CPP}/io_time.cpp"
            "${SHARED_FAKES_DIR_CPP}/io_imu.cpp"
            "${SHARED_FAKES_DIR_CPP}/io_shdnLoopNode.cpp"
            "${SHARED_FAKES_DIR_CPP}/io_pump.cpp"
    )
    set(SRCS ${APP_SRCS} ${APP_SHARED_SRCS} ${JOBS_SRC} ${TEST_SRCS} ${FAKE_SRCS})
    set(INCLUDE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/test"
            ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SHARED_UTIL_INCLUDE_DIR_CPP}
            ${SHARED_FAKES_DIR} ${SHARED_FAKES_DIR_CPP} ${SHARED_TEST_INCLUDE_DIR}
    )
    compile_gtest_executable(
            "seven_RSM_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    add_compile_definitions("seven_RSM_test" PRIVATE stm32h562Rx)

    # JSONCAN
    jsoncan_library_cpp(RSM seven "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "seven_RSM_test"
            "seven_RSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    # FakeIO
    set(HEADERS_TO_FAKE ${TEST_IO_HDRS}
    )

    file(GLOB TEST_IO_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.h")
    list(APPEND HEADERS_TO_FAKE ${TEST_IO_HDRS})
    create_fake_library(
            "seven_RSM_fakes"
            "${HEADERS_TO_FAKE}"
    )

    target_link_libraries("seven_RSM_test" PRIVATE "seven_RSM_jsoncan" "seven_RSM_fakes" "seven_RSM_commit_info")
    target_compile_definitions("seven_RSM_test" PRIVATE STM32H562xx)
endif()
