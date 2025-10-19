message("")
message("⚙️ Configuring VC")

set(ARM_CORE "cm7")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/VC.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

file(GLOB SYSTEM_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
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
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${CMAKE_CURRENT_SOURCE_DIR}/src/app/vehicle_dynamics" "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_QUADRUNA_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_canLogging.c"
        "${SHARED_IO_INCLUDE_DIR}/io_fileSystem_logfs.c"
        "${SHARED_IO_INCLUDE_DIR}/io_chimera.c"
        "${SHARED_IO_INCLUDE_DIR}/io_led.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_IO_INCLUDE_DIR}/io_bootHandler.c"
        "${SHARED_IO_INCLUDE_DIR}/io_taskMonitor.c"
        "${SHARED_IO_INCLUDE_DIR}/io_imu.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_QUADRUNA_DIR}")

file(GLOB_RECURSE HW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c")
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_uart.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_adc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_sd.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_ubsan.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can_h7.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${SHARED_HW_INCLUDE_DIR}"
        "${SHARED_HW_INCLUDE_QUADRUNA_DIR}"
)

IF ("${TARGET}" STREQUAL "binary")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})
    embedded_binary(
            "quadruna_VC_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    # Bootloader files.
    file(GLOB BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
    file(GLOB BOOT_CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
    set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot/" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
    # Build bootloader.
    stm32h7_boot_binary(
            "quadruna_VC_boot"
            "${BOOT_SRCS}"
            "${BOOT_CUBEMX_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "quadruna_VC"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c"
            "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/VC_boot.ioc"
    )


    generate_stm32cube_code(

            "${IOC_PATH}"
            "${CUBEMX_SRCS}"
    )

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
            "stm32h7xx_ll_sdmmc.c"
            "stm32h7xx_hal_sd_ex.c"
            "stm32h7xx_hal_sd.c"
            "stm32h7xx_hal.c"
            "stm32h7xx_hal_i2c.c"
            "stm32h7xx_hal_i2c_ex.c"
    )
    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32h733xx_cube_library(
            "quadruna_VC_stm32"
            "${STM32_HAL_SRCS}"
            "${SYSCALLS}"
            "${MD5_LOCATION}"
            FALSE
    )

    # Telemetry Protocol Buffers
    nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS proto/telem.proto)
    set(P_SRCS ${PROTO_SRCS})
    set(P_HDRS ${CMAKE_CURRENT_BINARY_DIR} ${NANOPB_INCLUDE_DIRS})
    embedded_library(
            "telem_proto"
            "${P_SRCS}"
            "${P_HDRS}"
            "cm7"
            TRUE
    )

    # JSONCAN
    jsoncan_embedded_library(VC quadruna ${CMAKE_CURRENT_BINARY_DIR})

    # CommitInfo
    commit_info_library(
            "quadruna_VC_app.elf"
            "quadruna_VC_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
    )

    target_link_libraries("quadruna_VC_app.elf" PRIVATE

            "quadruna_VC_stm32"
            "quadruna_VC_jsoncan"
            "quadruna_VC_commit_info"
            "logfs_cm7"
            "debug_modules"
            "m"
            "telem_proto"
            "sbg_ecom_cm7"
    )
    target_link_libraries("quadruna_VC_boot.elf" PRIVATE "quadruna_VC_commit_info")

    embedded_image(
            "quadruna_VC"
            "quadruna_VC_app.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quadruna_VC_app.hex"
            "quadruna_VC_boot.hex"
            "${CMAKE_CURRENT_BINARY_DIR}/quadruna_VC_boot.hex"
    )
ELSEIF ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test" "${SHARED_TEST_UTILS_INCLUDE_DIRS}")
    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
            "quadruna_VC_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    # JSONCAN
    jsoncan_library(VC quadruna "${CMAKE_CURRENT_BINARY_DIR}")

    # FakeIO
    set(HEADERS_TO_FAKE
            "${SHARED_IO_INCLUDE_DIR}/io_log.h"
            "${SHARED_IO_INCLUDE_DIR}/io_canLogging.h"
            "${SHARED_IO_INCLUDE_DIR}/io_taskMonitor.h"
            "${SHARED_IO_INCLUDE_DIR}/io_imu.h"
            "${SHARED_IO_INCLUDE_DIR}/io_time.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sbgEllipse.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_lowVoltageBattery.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_efuse.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_vcShdn.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_currentSensing.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sbgEllipse.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_potentiometer.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_telemMessage.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_pcm.h"
    )
    create_fake_library(
            "quadruna_VC_fakes"
            "${HEADERS_TO_FAKE}"
    )

    target_link_libraries("quadruna_VC_test" PRIVATE "quadruna_VC_jsoncan" "quadruna_VC_fakes")
ENDIF ()
