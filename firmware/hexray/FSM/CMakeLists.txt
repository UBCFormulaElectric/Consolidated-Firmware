message("")
message("⚙️ Configuring FSM (Hexray)")

file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.cpp")
set(TASKS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.cpp")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        #"${CMAKE_CURRENT_SOURCE_DIR}/boot"
)

file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.cpp")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_signal.c"
        "${SHARED_APP_INCLUDE_DIR}/app_shdnLoopNode.c"
        "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
        "${SHARED_APP_INCLUDE_DIR}/app_rangeCheck.c"
)
set(APP_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
        "${SHARED_APP_INCLUDE_DIR}"
        "${SHARED_APP_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_time.cpp"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
)
set(IO_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${SHARED_IO_INCLUDE_DIR}" 
        "${SHARED_IO_INCLUDE_DIR_CPP}"
)



file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.cpp"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_error.cpp"
)
set(HW_INCLUDE_DIRS
        #"${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}" 
        "${SHARED_HW_INCLUDE_DIR_CPP}"
)

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm33")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32h562ritx/stm32h562ritx_app_only.ld")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "hexray_FSM_app"
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
            "stm32h5xx_hal_adc.c"
            "stm32h5xx_hal_adc_ex.c"
            "stm32h5xx_hal_gpio.c"
            "stm32h5xx_hal_tim.c"
            "stm32h5xx_hal_tim_ex.c"
            "stm32h5xx_hal_i2c.c"
            "stm32h5xx_hal_i2c_ex.c"
            "stm32h5xx_hal_fdcan.c"
    )
    stm32h562xx_cube_library(
            "hexray_FSM_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/FSM.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDE_DIRS}"
            FALSE
            "${ARM_CORE}"
    )

    # JSONCAN
    jsoncan_embedded_library(FSM hexray "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "hexray_FSM_app.elf"
            "hexray_FSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("hexray_FSM_app.elf" PRIVATE "hexray_FSM_stm32" "hexray_FSM_commit_info"
            "hexray_FSM_jsoncan" "m")
    target_compile_definitions("hexray_FSM_app.elf" PRIVATE FSM)
elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(FAKE_SRCS 
        "${SHARED_FAKES_DIR}/io_imu.cpp"
        "${SHARED_FAKES_DIR}/io_time.cpp"
    )
    set(SRCS ${APP_SRCS} ${JOBS_SRC} ${TEST_SRCS} ${FAKE_SRCS})
    set(INCLUDE_DIRS
            # "${CMAKE_CURRENT_SOURCE_DIR}/test"
            ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} 
            ${SYSTEM_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR_CPP} 
            ${SHARED_UTIL_INCLUDE_DIR} ${SHARED_FAKES_DIR}
    )
    compile_gtest_executable(
            "hexray_FSM_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    add_compile_definitions("hexray_FSM_test" PRIVATE STM32H562Rx)

    # JSONCAN
    jsoncan_library(FSM hexray "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "hexray_FSM_test"
            "hexray_FSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("hexray_FSM_test" PRIVATE "hexray_FSM_commit_info" "hexray_FSM_jsoncan")
endif ()
