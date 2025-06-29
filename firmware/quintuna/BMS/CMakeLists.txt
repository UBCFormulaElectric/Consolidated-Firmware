message("")
message("⚙️ Configuring [BMS] (Quintuna)")

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

set(JOBS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs.c")
set(TASKS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.c")
set(SYSTEM_SRCS ${JOBS_SRC} ${TASKS_SRC})
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/boot")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
        "${SHARED_APP_INCLUDE_DIR}/app_thermistor.c"
        "${SHARED_APP_INCLUDE_DIR}/app_shdnLoopNode.c"
        "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
        "${SHARED_APP_INCLUDE_DIR}/app_jsoncan.c"
        "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
        "${SHARED_APP_INCLUDE_DIR}/app_math.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
        "${SHARED_IO_INCLUDE_DIR}/io_canQueue.c"
        "${SHARED_IO_INCLUDE_DIR}/io_time.c"
        "${SHARED_IO_INCLUDE_DIR}/io_bootHandler.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
)
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_usb.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_spi.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_adc.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_can_h7.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_pwmInput.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_chimera_v2.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_ubsan.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_resetReason.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_freeRtosConfigs.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}"
)

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm7")
    if ("${BOOTLOAD}" STREQUAL "ON")
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app.ld")
    else ()
        set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app_only.ld")
    endif ()
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})
    embedded_binary("quintuna_BMS_app" "${SRCS}" "${INCLUDE_DIRS}" "${LINKER_SCRIPT}" "${ARM_CORE}")

    set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/BMS.ioc")
    list(APPEND NO_CHECKS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/usbd_desc.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/usbd_cdc_if.c"
    )
    embedded_no_checks("${NO_CHECKS}")
    generate_stm32cube_code("quintuna_BMS_stm32cube" "${IOC_PATH}" "${CUBEMX_SRCS}")

    set(STM32_HAL_SRCS
            "stm32h7xx_hal_adc_ex.c"
            "stm32h7xx_hal_adc.c"
            "stm32h7xx_hal_cortex.c"
            "stm32h7xx_hal_dma_ex.c"
            "stm32h7xx_hal_dma.c"
            "stm32h7xx_hal_exti.c"
            "stm32h7xx_hal_fdcan.c"
            "stm32h7xx_hal_gpio.c"
            "stm32h7xx_hal_i2c_ex.c"
            "stm32h7xx_hal_i2c.c"
            "stm32h7xx_hal_iwdg.c"
            "stm32h7xx_hal_pwr_ex.c"
            "stm32h7xx_hal_rcc_ex.c"
            "stm32h7xx_hal_rcc.c"
            "stm32h7xx_hal_sd_ex.c"
            "stm32h7xx_hal_sd.c"
            "stm32h7xx_hal_spi.c"
            "stm32h7xx_hal_crc.c"
            "stm32h7xx_hal_crc_ex.c"
            "stm32h7xx_hal_pcd.c"
            "stm32h7xx_hal_pcd_ex.c"
            "stm32h7xx_hal_tim_ex.c"
            "stm32h7xx_hal_tim.c"
            "stm32h7xx_hal_uart_ex.c"
            "stm32h7xx_hal_uart.c"
            "stm32h7xx_hal.c"
            "stm32h7xx_ll_sdmmc.c"
            "stm32h7xx_hal_pcd.c"
            "stm32h7xx_hal_pcd_ex.c"
            "stm32h7xx_ll_usb.c"
    )
    stm32h733xx_cube_library(
            "quintuna_BMS_stm32cube_hal"
            "${STM32_HAL_SRCS}"
            "${MD5_LOCATION}"
            TRUE
    )

    # JSONCAN
    jsoncan_embedded_library(BMS quintuna "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

    # CommitInfo
    commit_info_library(
            "quintuna_BMS_app.elf"
            "quintuna_BMS_commit_info"
            "${CMAKE_CURRENT_BINARY_DIR}/app"
            "${ARM_CORE}"
    )

    target_link_libraries("quintuna_BMS_app.elf" PRIVATE "quintuna_BMS_stm32cube" "quintuna_BMS_stm32cube_hal" "quintuna_BMS_jsoncan" "quintuna_BMS_commit_info" "chimera_v2_proto_cm7")

    if ("${BOOTLOAD}" STREQUAL "ON")
        # Bootloader files.
        file(GLOB BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
        file(GLOB BOOT_CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/*.c")
        set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
        embedded_no_checks("${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c")
        stm32h7_boot_binary(
                "quintuna_BMS_boot"
                "${BOOT_SRCS}"
                "${BOOT_CUBEMX_SRCS}"
                "${BOOT_INCLUDE_DIRS}"
                "quintuna_BMS"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/BMS_boot.ioc"
        )
        target_link_libraries("quintuna_BMS_boot.elf" PRIVATE "quintuna_BMS_commit_info")

        embedded_image(
                "quintuna_BMS"
                "quintuna_BMS_app.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/quintuna_BMS_app.hex"
                "quintuna_BMS_boot.hex"
                "${CMAKE_CURRENT_BINARY_DIR}/quintuna_BMS_boot.hex"
        )
    endif ()
elseif ("${TARGET}" STREQUAL "test")
    file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

    set(SRCS ${APP_SRCS} ${JOBS_SRC} ${TEST_SRCS})
    set(INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test" ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})
    compile_gtest_executable(
            "quintuna_BMS_test"
            "${SRCS}"
            "${INCLUDE_DIRS}"
    )

    # JSONCAN
    jsoncan_library(BMS quintuna "${CMAKE_CURRENT_BINARY_DIR}")

    # FakeIO
    set(HEADERS_TO_FAKE
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_tractiveSystem.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_irs.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_imd.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_faultLatch.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_bspdTest.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_bmsShdn.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_charger.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_fans.h"
            "${SHARED_IO_INCLUDE_DIR}/io_time.h"
    )
    create_fake_library(
            "quintuna_BMS_fakes"
            "${HEADERS_TO_FAKE}"
    )

    target_link_libraries("quintuna_BMS_test" PRIVATE "quintuna_BMS_jsoncan" "quintuna_BMS_fakes")
    target_compile_definitions("quintuna_BMS_test" PRIVATE STM32H733xx)
endif ()
