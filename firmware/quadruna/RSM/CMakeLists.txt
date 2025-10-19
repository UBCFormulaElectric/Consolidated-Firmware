message("")
message("⚙️ Configuring RSM")

set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/RSM.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

file(GLOB SYSTEM_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/boot")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
        "${SHARED_APP_INCLUDE_DIR}/app_heartbeatMonitor.c"
        "${SHARED_APP_INCLUDE_DIR}/app_heartbeatBoard.c"
        "${SHARED_APP_INCLUDE_DIR}/app_rangeCheck.c"
        "${SHARED_APP_INCLUDE_DIR}/app_signal.c"
        "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
        "${SHARED_APP_INCLUDE_DIR}/app_utils.h"
        "${SHARED_APP_INCLUDE_DIR}/app_jsoncan.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_QUADRUNA_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_IO_INCLUDE_DIR}/io_chimera.c"
        "${SHARED_IO_INCLUDE_DIR}/io_led.c"
        "${SHARED_IO_INCLUDE_DIR}/io_mechanicalLoad.c"
        "${SHARED_IO_INCLUDE_DIR}/io_bootHandler.c"
        "${SHARED_IO_INCLUDE_DIR}/io_taskMonitor.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_QUADRUNA_DIR}")

file(GLOB_RECURSE HW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c")
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_adc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_pwmInputFreqOnly.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_ubsan.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can_f4.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
)
list(REMOVE_ITEM HW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/sysmem.c")
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${SHARED_HW_INCLUDE_DIR}"
        "${SHARED_HW_INCLUDE_QUADRUNA_DIR}"
)

IF ("${TARGET}" STREQUAL "binary")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${CUBEMX_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})
    embedded_binary(
            "quadruna_RSM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    # Bootloader files.
    file(GLOB BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    file(GLOB BOOT_CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
    list(REMOVE_ITEM BOOT_CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/sysmem.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot/" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
    # Build bootloader.
    stm32f4_boot_binary(
            "quadruna_RSM_boot"
            "${BOOT_SRCS}"
            "${BOOT_CUBEMX_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "quadruna_RSM"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/RSM_boot.ioc"
    )

    generate_stm32cube_code(

            "${IOC_PATH}"
            "${CUBEMX_SRCS}"
    )

    # STM HAL Library
    set(STM32_HAL_SRCS
            "stm32f4xx_hal_adc_ex.c"
            "stm32f4xx_hal_adc.c"
            "stm32f4xx_hal_cortex.c"
            "stm32f4xx_hal_dma_ex.c"
            "stm32f4xx_hal_dma.c"
            "stm32f4xx_hal_exti.c"
            "stm32f4xx_hal_can.c"
            "stm32f4xx_hal_gpio.c"
            "stm32f4xx_hal_iwdg.c"
            "stm32f4xx_hal_pwr_ex.c"
            "stm32f4xx_hal_rcc_ex.c"
            "stm32f4xx_hal_rcc.c"
            "stm32f4xx_hal_tim_ex.c"
            "stm32f4xx_hal_tim.c"
            "stm32f4xx_hal_uart.c"
            "stm32f4xx_hal.c"
    )
    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32f412rx_cube_library(
            "quadruna_RSM_stm32"
            "${STM32_HAL_SRCS}"
            "${SYSCALLS}"
            "${MD5_LOCATION}"
            FALSE
    )

    # JSONCAN
    jsoncan_embedded_library(RSM quadruna ${CMAKE_CURRENT_BINARY_DIR})

    # CommitInfo
    commit_info_library(
            "quadruna_RSM_app.elf"
            "quadruna_RSM_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("quadruna_RSM_app.elf" PRIVATE "quadruna_RSM_stm32" "quadruna_RSM_jsoncan" "quadruna_RSM_commit_info" "debug_modules" "m")
    target_link_libraries("quadruna_RSM_boot.elf" PRIVATE "quadruna_RSM_commit_info")

    embedded_image(
            "quadruna_RSM"
            "quadruna_RSM_app.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quadruna_RSM_app.hex"
            "quadruna_RSM_boot.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quadruna_RSM_boot.hex"
    )
elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test" "${SHARED_TEST_UTILS_INCLUDE_DIRS}")
    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
            "quadruna_RSM_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    # JSONCAN
    jsoncan_library(RSM quadruna "${CMAKE_CURRENT_BINARY_DIR}")

    # FakeIO
    set(HEADERS_TO_FAKE
            "${SHARED_IO_INCLUDE_DIR}/io_taskMonitor.h"
            "${SHARED_IO_INCLUDE_DIR}/io_time.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_coolant.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_loadCell.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_suspension.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_fans.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_brake_light.h"
    )
    create_fake_library(
            "quadruna_RSM_fakes"
            "${HEADERS_TO_FAKE}"
    )

    target_link_libraries("quadruna_RSM_test" PRIVATE "quadruna_RSM_jsoncan" "quadruna_RSM_fakes" "m")
ENDIF ()
