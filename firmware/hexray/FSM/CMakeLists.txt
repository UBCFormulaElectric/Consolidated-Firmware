message("")
message("⚙️ Configuring FSM (hexray)")

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.c")
set(TASKS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.c")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/boot")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
)
list(APPEND HW_SRCS)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}"
)

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm4")
    if ("${BOOTLOAD}" STREQUAL "ON")
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h563ritx/stm32h563ritx_app.ld")
    else ()
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h563ritx/stm32h563ritx_app_only.ld")
    endif ()
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})

    embedded_binary(
            "hexray_FSM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/FSM.ioc")
    list(APPEND NO_CHECKS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/usbd_desc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/usbd_cdc_if.c"
    )
    embedded_no_checks("${NO_CHECKS}")
    generate_stm32cube_code("hexray_FSM_stm32cube" "${IOC_PATH}" "${CUBEMX_SRCS}")

    set(STM32_HAL_SRCS
    )

    stm32f412rx_cube_library(
            "hexray_FSM_stm32cube_hal"
            "${STM32_HAL_SRCS}"
            "${MD5_LOCATION}"
            TRUE
    )

    # CommitInfo
    commit_info_library(
            "hexray_FSM_app.elf"
            "hexray_FSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
            "${ARM_CORE}"
    )

    target_link_libraries("hexray_FSM_app.elf" PRIVATE "hexray_FSM_stm32cube" "hexray_FSM_stm32cube_hal"
        "hexray_FSM_commit_info" "chimera_v2_proto_cm4" "m")

    if ("${BOOTLOAD}" STREQUAL "ON")
        #bootloader files
        file(GLOB BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
        file(GLOB BOOT_CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
        set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
        embedded_no_checks("${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c")
        stm32f4_boot_binary(
                "hexray_FSM_boot"
                "${BOOT_SRCS}"
                "${BOOT_CUBEMX_SRCS}"
                "${BOOT_INCLUDE_DIRS}"
                "hexray_FSM"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/FSM_boot.ioc"
        )
        target_link_libraries("hexray_FSM_boot.elf" PRIVATE "hexray_FSM_commit_info")

        embedded_image(
                "hexray_FSM"
                "hexray_FSM_app.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/hexray_FSM_app.hex"
                "hexray_FSM_boot.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/hexray_FSM_boot.hex"
        )
    endif ()
elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(SRCS ${APP_SRCS} ${JOBS_SRC} ${TEST_SRCS})
    set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test" ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})
    compile_gtest_executable(
            "hexray_FSM_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    add_compile_definitions("hexray_FSM_test" PRIVATE STM32F412Rx)

    # JSONCAN
    jsoncan_library(FSM hexray "${CMAKE_CURRENT_BINARY_DIR}")
    # CommitInfo
    commit_info_library(
            "hexray_FSM_test"
            "hexray_FSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )
    
    # FakeIO
    set(HEADERS_TO_FAKE
    )

    file(GLOB TEST_IO_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.h")
    list(APPEND HEADERS_TO_FAKE ${TEST_IO_HDRS})
    create_fake_library(
            "hexray_FSM_fakes"
            "${HEADERS_TO_FAKE}"
    )

    target_link_libraries("hexray_FSM_test" PRIVATE "hexray_FSM_fakes" "hexray_FSM_commit_info")
endif ()
