set(ARM_CORE "cm7")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/BMS.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32h733vgtx/stm32h733vgtx_app_only.ld")

# app
file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS 
    "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
    "${SHARED_APP_INCLUDE_DIR}/app_heartbeatMonitor.c"
    "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
    "${SHARED_APP_INCLUDE_DIR}/app_math.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

# io
file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")
list(APPEND IO_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c"
    "${SHARED_IO_INCLUDE_DIR}/io_time.c"
    "${SHARED_IO_INCLUDE_DIR}/io_chimera.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

# hw
file(GLOB_RECURSE HW_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
list(APPEND HW_SRCS 
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_fdcan.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_adcConversions.c"
    # "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_spi.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_pwmInput.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_stackWaterMark.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_uart.c" # TODO: add back when uart finalized
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SHARED_HW_INCLUDE_DIR}")

# tests
file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "quadruna_BMS_can"
        "BMS"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
        "quadruna"
    )
    embedded_library(
        quadruna_BMS_can
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )

    generate_stm32cube_code(
        "quadruna_BMS"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        "stm32h7xx_hal_adc_ex.c"
        "stm32h7xx_hal_adc.c"
        "stm32h7xx_hal_fdcan.c"
        "stm32h7xx_hal_cortex.c"
        "stm32h7xx_hal_dma_ex.c"
        "stm32h7xx_hal_dma.c"
        "stm32h7xx_hal_exti.c"
        "stm32h7xx_hal_gpio.c"
        "stm32h7xx_hal_i2c.c"
        "stm32h7xx_hal_iwdg.c"
        "stm32h7xx_hal_pwr_ex.c"
        "stm32h7xx_hal_rcc_ex.c"
        "stm32h7xx_hal_rcc.c"
        "stm32h7xx_hal_spi.c"
        "stm32h7xx_hal_tim_ex.c"
        "stm32h7xx_hal_tim.c"
        "stm32h7xx_hal_uart_ex.c"
        "stm32h7xx_hal_uart.c"
        "stm32h7xx_hal_sd.c"
        "stm32h7xx_ll_sdmmc.c"
        "stm32h7xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32h733xx_cube_library(
        "quadruna_BMS_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    embedded_binary(
        "quadruna_BMS_app"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )

    # Autogenerate git commit info header/source.
    commit_info_library(
        "quadruna_BMS_app.elf"
        "quadruna_BMS_commit_info"
        "${CMAKE_CURRENT_BINARY_DIR}/app"
        "${ARM_CORE}"
    )

    target_link_libraries("quadruna_BMS_can" "quadruna_BMS_stm32cube")
    target_link_libraries("quadruna_BMS_app.elf" "quadruna_BMS_stm32cube" "quadruna_BMS_can" "quadruna_BMS_commit_info" "debug_modules")

     embedded_image(
         "quadruna_BMS"
         "quadruna_BMS_app.hex"
         "${CMAKE_CURRENT_BINARY_DIR}/quadruna_BMS_app.hex"
         "quadruna_BMS_boot.hex"
         "${CMAKE_BINARY_DIR}/firmware/boot/h7boot/quadruna_BMS_boot.hex"
     )

elseif("${TARGET}" STREQUAL "test")
set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/ltc6813/io_ltc6813CellTemps.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/ltc6813/io_ltc6813CellVoltages.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/ltc6813/io_ltc6813Shared.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_airs.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_charger.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_faultLatch.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_imd.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_thermistors.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_tractiveSystem.h"
    )
    create_fake_library(
        "quadruna_BMS_fakes"
        "${HEADERS_TO_FAKE}"
    )
    jsoncan_library(
        "quadruna_BMS_can"
        "BMS"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
        "quadruna"
    )
    add_library(quadruna_BMS_can STATIC "${CAN_SRCS}")
    target_include_directories(quadruna_BMS_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
        "quadruna_BMS_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("quadruna_BMS_test" "quadruna_BMS_can" "quadruna_BMS_fakes" "shared_fakes")
endif()
