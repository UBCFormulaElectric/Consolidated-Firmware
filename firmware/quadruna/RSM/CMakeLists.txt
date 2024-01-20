set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/RSM.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS "${SHARED_APP_SRCS}")
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
list(APPEND IO_SRCS "${SHARED_IO_SRCS}")
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

set(HW_SRCS "${SHARED_HW_SRCS}")
list(REMOVE_ITEM HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_sd.c")
list(REMOVE_ITEM HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_crc.c")
list(REMOVE_ITEM HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_flash.c")
list(REMOVE_ITEM HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_fdcan.c")

file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

# Autogenerate git commit info header/source.
set(COMMIT_INFO_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.h")
set(COMMIT_INFO_SRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.c")
commit_info_library(
    "RSM_commit_info"
    "${COMMIT_INFO_HEADER_PATH}"
    "${COMMIT_INFO_SRC_PATH}"
    "${ARM_CORE}"
)

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "RSM_can"
        "RSM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
    )
    embedded_library(
        RSM_can
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )

    generate_stm32cube_code(
        "RSM"
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
        "stm32f4xx_hal_i2c_ex.c"
        "stm32f4xx_hal_i2c.c"
        "stm32f4xx_hal_iwdg.c"
        "stm32f4xx_hal_rcc_ex.c"
        "stm32f4xx_hal_rcc.c"
        "stm32f4xx_hal_spi.c"
        "stm32f4xx_hal_tim_ex.c"
        "stm32f4xx_hal_tim.c"
        "stm32f4xx_hal_uart.c"
        "stm32f4xx_hal.c"
        "stm32f4xx_ll_adc.c"
    )
    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32f412rx_cube_library(
        "RSM_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${SHARED_HW_INCLUDE_DIR})
    embedded_binary(
        "RSM"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )
    target_link_libraries("RSM_can" "RSM_stm32cube")
    target_link_libraries("RSM.elf" "RSM_stm32cube" "RSM_can" "RSM_commit_info")

   
elseif("${TARGET}" STREQUAL "test")
    jsoncan_library(
        "RSM_can"
        "RSM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
    )
    add_library(RSM_can STATIC "${CAN_SRCS}")
    target_include_directories(RSM_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${FFF_DIR})
    compile_gtest_executable(
        "RSM_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("RSM_test" "RSM_can" "shared_fakes")
endif()