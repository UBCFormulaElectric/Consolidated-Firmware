message("")
message("⚙️ Configuring RSM (Hexray)")

file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.cpp")
set(TASKS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.cpp")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        #        "${CMAKE_CURRENT_SOURCE_DIR}/boot"
)

file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.cpp")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_signal.c"
        "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
        "${SHARED_APP_INCLUDE_DIR}/app_rangeCheck.c"
)
set(APP_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
        "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
list(APPEND IO_SRCS)
set(IO_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_DIR_CPP}"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_i2c.cpp")
set(HW_INCLUDE_DIRS
        #        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_DIR_CPP}"
)

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm33")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32h562ritx/stm32h562ritx_app_only.ld")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "hexray_RSM_app"
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
            "stm32h5xx_hal_i2c.c"
            "stm32h5xx_hal_i2c_ex.c"
            "stm32h5xx_hal_adc.c"
            "stm32h5xx_hal_adc_ex.c"
            "stm32h5xx_hal_gpio.c"
    )
    stm32h562xx_cube_library(
            "hexray_RSM_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/RSM.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDE_DIRS}"
            FALSE
            "${ARM_CORE}"
    )

    # JSONCAN
    jsoncan_embedded_library(RSM hexray "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "hexray_RSM_app.elf"
            "hexray_RSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("hexray_RSM_app.elf" PRIVATE "hexray_RSM_stm32" "hexray_RSM_commit_info" "hexray_RSM_jsoncan" "m")

elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(FAKE_SRCS
        "${SHARED_FAKES_DIR}/io_time.cpp"
        #"${SHARED_FAKES_DIR}/io_imu.cpp"
    )
    set(SRCS ${APP_SRCS} ${JOBS_SRC} ${TEST_SRCS} ${FAKE_SRCS})
    set(INCLUDE_DIRS
            # "${CMAKE_CURRENT_SOURCE_DIR}/test"
            ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SHARED_UTIL_INCLUDE_DIR_CPP}
            ${SHARED_FAKES_DIR} ${SHARED_TEST_INCLUDE_DIR}
    )
    compile_gtest_executable(
            "hexray_RSM_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    add_compile_definitions("hexray_RSM_test" PRIVATE STM32H562Rx)

    # JSONCAN
    jsoncan_library(RSM hexray "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "hexray_RSM_test"
            "hexray_RSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    # FakeIO
    set(HEADERS_TO_FAKE)

    file(GLOB TEST_IO_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.h")
    list(APPEND HEADERS_TO_FAKE ${TEST_IO_HDRS})
    create_fake_library(
            "hexray_RSM_fakes"
            "${HEADERS_TO_FAKE}"
    )

    target_link_libraries("hexray_RSM_test" PRIVATE "hexray_RSM_fakes" "hexray_RSM_commit_info" "hexray_RSM_jsoncan")
endif ()
