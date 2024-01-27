set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/FSM.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS 
    "${SHARED_APP_INCLUDE_DIR}/App_SharedStateMachine.c"
    "${SHARED_APP_INCLUDE_DIR}/App_SharedHeartbeatMonitor.c"
    "${SHARED_APP_INCLUDE_DIR}/App_InRangeCheck.c"
    "${SHARED_APP_INCLUDE_DIR}/App_SharedSignal.c"
    "${SHARED_APP_INCLUDE_DIR}/App_Timer.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c"
    "${SHARED_IO_INCLUDE_DIR}/Io_SharedSoftwareWatchdog.c"
    "${SHARED_IO_INCLUDE_DIR}/Io_SharedStackWaterMark.c"
    "${SHARED_IO_INCLUDE_DIR}/Io_SharedHeartbeatMonitor.c"
    "${SHARED_IO_INCLUDE_DIR}/Io_SharedFreqOnlyPwmInput.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
list(APPEND HW_SRCS 
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_adcConversions.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
)
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${SHARED_HW_INCLUDE_DIR}")

file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/test")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

# Autogenerate git commit info header/source.
if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "FSM_can"
        "FSM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
        "thruna"
    )
    embedded_library(
        FSM_can
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )

    generate_stm32cube_code(
        "FSM"
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
        "FSM_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    embedded_binary(
        "FSM_app"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )

    set(COMMIT_INFO_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.h")
    set(COMMIT_INFO_SRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.c")
    commit_info_library(
        "FSM_app.elf"
        "FSM_commit_info"
        "${COMMIT_INFO_HEADER_PATH}"
        "${COMMIT_INFO_SRC_PATH}"
        "${ARM_CORE}"
    )

    target_link_libraries("FSM_can" "FSM_stm32cube")
    target_link_libraries("FSM_app.elf" "FSM_stm32cube" "FSM_can" "FSM_commit_info" "m")

    embedded_image(
        "FSM"
        "FSM_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/FSM_app.hex"
        "FSM_boot.hex"
        "${CMAKE_BINARY_DIR}/firmware/boot/f4boot/FSM_boot.hex"
    )

elseif("${TARGET}" STREQUAL "test")
    jsoncan_library(
        "FSM_can"
        "FSM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
        "thruna"
    )
    add_library(FSM_can STATIC "${CAN_SRCS}")
    target_include_directories(FSM_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_apps.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_brake.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_coolant.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_steering.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_wheels.h"
    )
    create_fake_library(
        "FSM_fakes"
        "${HEADERS_TO_FAKE}"
    )

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
        "FSM_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("FSM_test" "FSM_can" "FSM_fakes" "FSM_commit_info" "shared_fakes")
endif()
