message("")
message("⚙️ Configuring CRIT (Hexray)")

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

file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR_CPP}/io_time.cpp"
)

set(IO_INCLUDE_DIRS
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE HW_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c")
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_ubsan.cpp"
)
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
            "hexray_CRIT_app"
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
    )
    stm32h562xx_cube_library(
            "hexray_CRIT_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/CRIT.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDE_DIRS}"
            FALSE
            "${ARM_CORE}"
    )

    # JSONCAN
    jsoncan_embedded_library(CRIT hexray "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "hexray_CRIT_app.elf"
            "hexray_CRIT_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("hexray_CRIT_app.elf" PRIVATE "hexray_CRIT_stm32" "hexray_CRIT_commit_info" "hexray_CRIT_jsoncan" "m")
    target_compile_definitions("hexray_CRIT_app.elf" PRIVATE CRIT)
elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(SRCS ${APP_SRCS} ${JOBS_SRC} ${TEST_SRCS})
    set(INCLUDE_DIRS
            # "${CMAKE_CURRENT_SOURCE_DIR}/test"
            ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR_CPP}
    )
    compile_gtest_executable(
            "hexray_CRIT_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    add_compile_definitions("hexray_CRIT_test" PRIVATE STM32H562Rx)

    # JSONCAN
    jsoncan_library(CRIT hexray "${CMAKE_CURRENT_BINARY_DIR}")

    # CommitInfo
    commit_info_library(
            "hexray_CRIT_test"
            "hexray_CRIT_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    # FakeIO
    set(HEADERS_TO_FAKE
    )

    file(GLOB TEST_IO_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.h")
    list(APPEND HEADERS_TO_FAKE ${TEST_IO_HDRS})
    create_fake_library(
            "hexray_CRIT_fakes"
            "${HEADERS_TO_FAKE}"
    )

    target_link_libraries("hexray_CRIT_test" PRIVATE "hexray_CRIT_fakes" "hexray_CRIT_commit_info" "hexray_CRIT_jsoncan")
endif ()
