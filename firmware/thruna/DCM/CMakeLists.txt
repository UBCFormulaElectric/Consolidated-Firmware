set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/DCM.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/App/*.c")
list(APPEND APP_SRCS 
    "${SHARED_APP_INCLUDE_DIR}/App_SharedStateMachine.c"
    "${SHARED_APP_INCLUDE_DIR}/App_SharedHeartbeatMonitor.c"
    "${SHARED_APP_INCLUDE_DIR}/App_Timer.c"
    "${SHARED_APP_INCLUDE_DIR}/App_SharedWaitSignal.c"
    "${SHARED_APP_INCLUDE_DIR}/App_SharedClock.c"
    "${SHARED_APP_INCLUDE_DIR}/App_InRangeCheck.c"
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/App" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/Io/*.c")
list(APPEND IO_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c"
    "${SHARED_IO_INCLUDE_DIR}/Io_SharedSoftwareWatchdog.c"
    "${SHARED_IO_INCLUDE_DIR}/Io_SharedStackWaterMark.c"
    "${SHARED_IO_INCLUDE_DIR}/Io_SharedHeartbeatMonitor.c"
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/Io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c")
list(APPEND HW_SRCS 
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_bootup.c"
)

file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

# Autogenerate git commit info header/source.
set(COMMIT_INFO_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.h")
set(COMMIT_INFO_SRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.c")
commit_info_library(
    "DCM_commit_info"
    "${COMMIT_INFO_HEADER_PATH}"
    "${COMMIT_INFO_SRC_PATH}"
    "${ARM_CORE}"
)

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "DCM_can"
        "DCM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
    )
    embedded_library(
        DCM_can
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )

    generate_stm32cube_code(
        "DCM"
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
        "stm32f4xx_hal_uart.c"
        "stm32f4xx_hal.c"
    )
    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/Src/syscalls.c")
    stm32f412rx_cube_library(
        "DCM_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )
    
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${SHARED_HW_INCLUDE_DIR})
    embedded_binary(
        "DCM_app"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    ) 
    target_link_libraries(DCM_app.elf m)
    target_link_libraries("DCM_can" "DCM_stm32cube")
    target_link_libraries("DCM_app.elf" "DCM_stm32cube" "DCM_can" "DCM_commit_info" "sbg_ecom_cm4")

    embedded_image(
        "DCM"
        "DCM_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/DCM_app.hex"
        "DCM_boot.hex"
        "${CMAKE_BINARY_DIR}/firmware/boot/f4boot/DCM_boot.hex"
    )

elseif("${TARGET}" STREQUAL "test")
    jsoncan_library(
        "DCM_can"
        "DCM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
    )
    add_library(DCM_can STATIC "${CAN_SRCS}")
    target_include_directories(DCM_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${FFF_DIR})
    compile_gtest_executable(
        "DCM_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("DCM_test" "DCM_can" "DCM_commit_info" "shared_fakes")
endif()
