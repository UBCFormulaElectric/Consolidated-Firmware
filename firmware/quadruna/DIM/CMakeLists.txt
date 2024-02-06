set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DIM.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS 
    "${SHARED_APP_INCLUDE_DIR}/App_SharedStateMachine.c"
    "${SHARED_APP_INCLUDE_DIR}/App_SharedHeartbeatMonitor.c"
    "${SHARED_APP_INCLUDE_DIR}/App_Timer.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_rgbLed.c"
    "${SHARED_IO_INCLUDE_DIR}/io_led.c"
    "${SHARED_IO_INCLUDE_DIR}/io_switch.c"
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c"
    "${SHARED_IO_INCLUDE_DIR}/io_time.c"
    "${SHARED_IO_INCLUDE_DIR}/Io_SharedSoftwareWatchdog.c"
    "${SHARED_IO_INCLUDE_DIR}/Io_SharedStackWaterMark.c"
    "${SHARED_IO_INCLUDE_DIR}/Io_SharedHeartbeatMonitor.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
list(APPEND HW_SRCS 
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${SHARED_HW_INCLUDE_DIR}")

file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")
list(REMOVE_ITEM HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_uart.c")

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "DIM_can"
        "DIM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
        "thruna"
    )
    embedded_library(
        DIM_can
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )

    message("${IOC_PATH}")
    generate_stm32cube_code(
        "DIM"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        "stm32f4xx_hal_can.c"
        "stm32f4xx_hal_cortex.c"
        "stm32f4xx_hal_dma_ex.c"
        "stm32f4xx_hal_dma.c"
        "stm32f4xx_hal_exti.c"
        "stm32f4xx_hal_gpio.c"
        "stm32f4xx_hal_iwdg.c"
        "stm32f4xx_hal_rcc_ex.c"
        "stm32f4xx_hal_rcc.c"
        "stm32f4xx_hal_tim_ex.c"
        "stm32f4xx_hal_tim.c"
        "stm32f4xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32f412rx_cube_library(
        "DIM_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    embedded_binary(
        DIM_app
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )

        commit_info_library(
        "DIM_app.elf"
        "DIM_commit_info"
        "${CMAKE_CURRENT_BINARY_DIR}/app"
        "${ARM_CORE}"
    )
    target_link_libraries("DIM_can" "DIM_stm32cube")
    target_link_libraries("DIM_app.elf" "DIM_can" "DIM_stm32cube" "DIM_commit_info")
    
    embedded_image(
        "DIM"
        "DIM_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/DIM_app.hex"
        "DIM_boot.hex"
        "${CMAKE_BINARY_DIR}/firmware/boot/f4boot/DIM_boot.hex"
    )

elseif("${TARGET}" STREQUAL "test")
    set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sevenSegDisplays.h"
    )
    create_fake_library(
        "DIM_fakes"
        "${HEADERS_TO_FAKE}"
    )

    jsoncan_library(
        "DIM_can"
        "DIM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
        "thruna"
    )
    add_library(DIM_can STATIC "${CAN_SRCS}")
    target_include_directories(DIM_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
        "DIM_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("DIM_test" "DIM_can" "DIM_fakes" "DIM_commit_info" "shared_fakes")
endif()
