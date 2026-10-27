message("")
message("⚙️ Configuring VC (Seven)")

# Cmake Configuration to glob cubemx source code
# TODO: Add project specific and shared sources used in project
file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "")
set(CUBEMX_INCLUDE_DIRS "")

# Task and Job level source file glob
# TODO: Add project specific and shared sources used in project
set(JOBS_SRC "")
set(TASKS_SRC "")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS "" "")

# APP layer resources (shared + project specific APP code)
# TODO: Add project specific and shared sources used in project
file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "")
set(APP_SHARED_SRCS
)
set(APP_INCLUDE_DIRS
)

# IO layer resources (shared + project specific IO code)
# TODO: Add project specific and shared sources used in project
file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "")
set(IO_SHARED_SRCS
)
set(IO_INCLUDE_DIRS
)

# IO layer resources (shared + project specific HW code)
# TODO: Add project specific and shared sources used in project
file(GLOB_RECURSE HW_SRCS CONFIGURE_DEPENDS "")
set(HW_SHARED_SRCS
)
set(HW_INCLUDE_DIRS
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
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")
    else ()
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app_only.ld")
    endif ()
    set(SRCS ${APP_SRCS} ${APP_SHARED_SRCS} ${IO_SRCS} ${IO_SHARED_SRCS} ${HW_SRCS} ${HW_SHARED_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "seven_VC_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    #TODO: Add necessary HAL source if require new driver
    set(STM32_HAL_SRCS
            "stm32h7xx_hal.c"
            "stm32h7xx_hal_rcc.c"
            "stm32h7xx_hal_rcc_ex.c"
            "stm32h7xx_hal_pwr_ex.c"
            "stm32h7xx_hal_cortex.c"
            "stm32h7xx_hal_exti.c"
            "stm32h7xx_hal_tim.c"
            "stm32h7xx_hal_gpio.c"
            "stm32h7xx_hal_adc.c"
            "stm32h7xx_hal_dma.c"
            "stm32h7xx_hal_iwdg.c"

            "stm32h7xx_hal_adc_ex.c"
            "stm32h7xx_hal_tim_ex.c"
            "stm32h7xx_hal_dma_ex.c"

            "stm32h7xx_hal_crc.c"
            "stm32h7xx_hal_crc_ex.c"
            "stm32h7xx_hal_fdcan.c"
            "stm32h7xx_hal_spi.c"
            "stm32h7xx_hal_spi_ex.c"

            "stm32h7xx_hal_sd.c"
            "stm32h7xx_hal_sd_ex.c"
            "stm32h7xx_ll_sdmmc.c"

            "stm32h7xx_hal_pcd.c"
            "stm32h7xx_hal_pcd_ex.c"
            "stm32h7xx_ll_usb.c"

            "stm32h7xx_hal_i2c.c"
    )

    set(CUBEMX_INCLUDES ${CUBEMX_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    stm32h733xx_cube_library(
            "seven_VC_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/VC.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDES}"
            TRUE
            "${ARM_CORE}"
    )

    # JSONCAN
    jsoncan_embedded_library_cpp(VC seven "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})


    # CommitInfo
    commit_info_library(
            "seven_VC_app.elf"
            "seven_VC_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("seven_VC_app.elf" PRIVATE "seven_VC_stm32" "seven_VC_jsoncan" "seven_VC_commit_info" "m")
    target_compile_definitions("seven_VC_app.elf" PRIVATE VC)

    add_chimera_stm32h7("seven_VC_chimera" "${CHIMERA_SRCS}" "${CHIMERA_INCLUDE_DIRS}" "seven_chimera_v2_proto_cm7")
    target_link_libraries("seven_VC_chimera.elf" PRIVATE "seven_VC_stm32")

    if ("${BOOTLOAD}" STREQUAL "ON")
        target_compile_definitions("seven_VC_app.elf" PRIVATE BOOTLOAD)
        # TODO: Add project specific and shared sources used in project for bootloader

        file(GLOB BOOT_SRCS CONFIGURE_DEPENDS "")
        file(GLOB BOOT_CUBEMX_SRCS CONFIGURE_DEPENDS "")
        set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "")
        no_checks("")
        stm32h7_boot_binary(
                "seven_VC_boot"
                "${BOOT_SRCS}"
                "${BOOT_CUBEMX_SRCS}"
                "${BOOT_INCLUDE_DIRS}"
                "seven_VC"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/VC_boot.ioc"
        )
        target_link_libraries("seven_VC_boot.elf" PRIVATE "seven_VC_commit_info")
        embedded_image(
                "seven_VC"
                "seven_VC_app.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/seven_VC_app.hex"
                "seven_VC_boot.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/seven_VC_boot.hex"
        )
    endif ()

elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS "")
    set(FAKE_SRCS
    )
    set(SRCS ${APP_SRCS} ${APP_SHARED_SRCS} ${JOBS_SRC} ${TEST_SRCS} ${FAKE_SRCS})
    set(INCLUDE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/test"
            ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR_CPP} ${SHARED_FAKES_DIR}
    )
    compile_gtest_executable(
            "seven_VC_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    add_compile_definitions("seven_VC_test" PRIVATE STM32H733Rx)

    # JSONCAN
    jsoncan_library_cpp(VC seven "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "seven_VC_test"
            "seven_VC_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    # FakeIO
    set(HEADERS_TO_FAKE
    )
    create_fake_library(
            "seven_VC_fakes"
            "${HEADERS_TO_FAKE}"
    )

    target_link_libraries("seven_VC_test" PRIVATE "seven_VC_jsoncan" "seven_VC_fakes" "seven_VC_commit_info")
    target_compile_definitions("seven_VC_test" PRIVATE STM32H733xx)
endif()
