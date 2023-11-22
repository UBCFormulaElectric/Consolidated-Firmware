set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/BMS.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/App/*.c")
list(APPEND APP_SRCS "${SHARED_APP_SRCS}")
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/App" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Io/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c"
)
list(APPEND IO_SRCS "${SHARED_IO_SRCS}")
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/Io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")
list(REMOVE_ITEM SHARED_HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_sd.c")
list(REMOVE_ITEM SHARED_HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_crc.c")
list(REMOVE_ITEM SHARED_HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_flash.c")

# Autogenerate git commit info header/source.
set(COMMIT_INFO_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.h")
set(COMMIT_INFO_SRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.c")
commit_info_library(
    "BMS_commit_info"
    "${COMMIT_INFO_HEADER_PATH}"
    "${COMMIT_INFO_SRC_PATH}"
    "${ARM_CORE}"
)

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "BMS_can"
        "BMS"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
    )
    embedded_library(
        BMS_can
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
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/Src/syscalls.c")
    stm32f412rx_cube_library(
        "BMS_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${SHARED_HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${SHARED_HW_INCLUDE_DIR})
    embedded_binary(
        "BMS_app"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )
    target_link_libraries("BMS_can" "BMS_stm32cube")
    target_link_libraries("BMS_app.elf" "BMS_stm32cube" "BMS_can" "BMS_commit_info")

    embedded_image(
        "BMS"
        "BMS_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/BMS_app.hex"
        "BMS_boot.hex"
        "${CMAKE_BINARY_DIR}/firmware/boot/f4boot/BMS_boot.hex"
    )

elseif("${TARGET}" STREQUAL "test")
    jsoncan_library(
        "BMS_can"
        "BMS"
        "${CMAKE_CURRENT_BINARY_DIR}"
        FALSE
    )
    add_library(BMS_can STATIC "${CAN_SRCS}")
    target_include_directories(BMS_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${FFF_DIR})
    compile_gtest_executable(
        "BMS_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("BMS_test" "BMS_can" "shared_fakes")
endif()