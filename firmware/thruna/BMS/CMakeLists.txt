set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/BMS.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS 
    "${SHARED_APP_INCLUDE_DIR}/app_stateMachine.c"
    "${SHARED_APP_INCLUDE_DIR}/app_timer.c"
    "${SHARED_APP_INCLUDE_DIR}/app_math.c"
    "${SHARED_APP_INCLUDE_DIR}/app_heartbeatMonitor.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}" "${SHARED_APP_INCLUDE_THRUNA_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c"
    "${SHARED_IO_INCLUDE_DIR}/io_time.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}" "${SHARED_IO_INCLUDE_THRUNA_DIR}")

file(GLOB_RECURSE HW_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
list(APPEND HW_SRCS 
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_adcConversions.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_spi.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_pwmInput.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_watchdog.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_stackWaterMark.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SHARED_HW_INCLUDE_DIR}" "${SHARED_HW_INCLUDE_THRUNA_DIR}")

file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "thruna_BMS_can"
        "BMS"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
        "thruna"
    )
    embedded_library(
        thruna_BMS_can
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )

    generate_stm32cube_code(
        "BMS"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        "stm32f4xx_hal_adc_ex.c"
        "stm32f4xx_hal_adc.c"
        "stm32f4xx_hal_can.c"
        "stm32f4xx_hal_cortex.c"
        "stm32f4xx_hal_dma_ex.c"
        "stm32f4xx_hal_dma.c"
        "stm32f4xx_hal_exti.c"
        "stm32f4xx_hal_gpio.c"
        "stm32f4xx_hal_i2c.c"
        "stm32f4xx_hal_iwdg.c"
        "stm32f4xx_hal_rcc_ex.c"
        "stm32f4xx_hal_rcc.c"
        "stm32f4xx_hal_spi.c"
        "stm32f4xx_hal_tim_ex.c"
        "stm32f4xx_hal_tim.c"
        "stm32f4xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32f412rx_cube_library(
        "thruna_BMS_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    embedded_binary(
        "thruna_BMS_app"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )

    # Autogenerate git commit info header/source.
    commit_info_library(
        "thruna_BMS_app.elf"
        "thruna_BMS_commit_info"
        "${CMAKE_CURRENT_BINARY_DIR}/app"
        "${ARM_CORE}"
    )

    target_link_libraries("thruna_BMS_can" "thruna_BMS_stm32cube")
    target_link_libraries("thruna_BMS_app.elf" "thruna_BMS_stm32cube" "thruna_BMS_can" "thruna_BMS_commit_info")

    embedded_image(
        "thruna_BMS"
        "thruna_BMS_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/thruna_BMS_app.hex"
        "thruna_BMS_boot.hex"
        "${CMAKE_BINARY_DIR}/firmware/boot/f4boot/thruna_BMS_boot.hex"
    )

elseif("${TARGET}" STREQUAL "test")
set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/ltc6813/io_ltc6813CellTemps.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/ltc6813/io_ltc6813CellVoltages.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/ltc6813/io_ltc6813Shared.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_airs.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_charger.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_eeprom.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_faultLatch.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_imd.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_thermistors.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_tractiveSystem.h"
    )
    create_fake_library(
        "thruna_BMS_fakes"
        "${HEADERS_TO_FAKE}"
        ""
    )
    jsoncan_library(
        "thruna_BMS_can"
        "BMS"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
        "thruna"
    )
    add_library(thruna_BMS_can STATIC "${CAN_SRCS}")
    target_include_directories(thruna_BMS_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
        "thruna_BMS_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("thruna_BMS_test" "thruna_BMS_can" "thruna_BMS_fakes" "shared_fakes")
endif()
