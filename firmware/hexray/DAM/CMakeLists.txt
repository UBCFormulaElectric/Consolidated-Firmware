message("")
message("⚙️ Configuring DAM (Hexray)")

file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.cpp")
set(TASKS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.cpp")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        #        "${CMAKE_CURRENT_SOURCE_DIR}/boot"
)

file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS)
set(APP_INCLUDE_DIRS
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
        "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_time.cpp"
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_log.cpp"
)
set(IO_INCLUDE_DIRS
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.cpp"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_error.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_sd.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_usb.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_gpio.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_can_hx.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_hardFaultHandler.cpp"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_DIR_CPP}"
)

# chimera sources
set(CHIMERA_SRCS ${HW_SRCS} ${IO_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/src/chimera.cpp")
set(CHIMERA_INCLUDE_DIRS ${HW_INCLUDE_DIRS} ${IO_INCLUDE_DIRS})

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm33")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32h562ritx/stm32h562ritx_app_only.ld")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "hexray_DAM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    set(STM32_HAL_SRCS
            "stm32h5xx_hal.c"
            "stm32h5xx_hal_rcc.c"
            "stm32h5xx_hal_rcc_ex.c"
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
    )
    set(CUBEMX_INCLUDE_DIRS_ ${CUBEMX_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})
    stm32h562xx_cube_library(
            "hexray_DAM_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DAM.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDE_DIRS_}"
            TRUE
            "${ARM_CORE}"
    )

    # JSONCAN
    jsoncan_embedded_library(DAM hexray "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "hexray_DAM_app.elf"
            "hexray_DAM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("hexray_DAM_app.elf" PRIVATE "hexray_DAM_stm32" "hexray_DAM_commit_info" "hexray_DAM_jsoncan" "m")
    target_compile_definitions("hexray_DAM_app.elf" PRIVATE DAM)

    add_chimera_stm32h5("hexray_DAM_chimera" "${CHIMERA_SRCS}" "${CHIMERA_INCLUDE_DIRS}" "hexray_chimera_v2_proto_cm33")
    target_link_libraries("hexray_DAM_chimera.elf" PRIVATE "hexray_DAM_stm32")
elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(SRCS ${APP_SRCS} ${JOBS_SRC} ${TEST_SRCS})
    set(INCLUDE_DIRS
            # "${CMAKE_CURRENT_SOURCE_DIR}/test"
            ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR_CPP}
    )
    compile_gtest_executable(
            "hexray_DAM_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    add_compile_definitions("hexray_DAM_test" PRIVATE STM32H562Rx)

    # JSONCAN
    jsoncan_library(DAM hexray "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "hexray_DAM_test"
            "hexray_DAM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    # FakeIO
    set(HEADERS_TO_FAKE
    )

    file(GLOB TEST_IO_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.h")
    list(APPEND HEADERS_TO_FAKE ${TEST_IO_HDRS})
    create_fake_library(
            "hexray_DAM_fakes"
            "${HEADERS_TO_FAKE}"
    )

    target_link_libraries("hexray_DAM_test" PRIVATE "hexray_DAM_fakes" "hexray_DAM_commit_info" "hexray_DAM_jsoncan")
endif ()
