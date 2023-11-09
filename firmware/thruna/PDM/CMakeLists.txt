set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/PDM.ioc")
set(LINKER_SCRIPT "${THIRD_PARTY_DIR}/linker/STM32F412RGTX_FLASH.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Src/App/*.c")
list(APPEND APP_SRCS "${SHARED_APP_SRCS}")
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/App" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Io/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c"
)
list(APPEND IO_SRCS "${SHARED_IO_SRCS}")
list(REMOVE_ITEM IO_SRCS "${SHARED_IO_INCLUDE_DIR}/Io_SharedSpi.c")
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Inc/Io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")
list(REMOVE_ITEM SHARED_HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_sd.c")

# Autogenerate git commit info header/source.
set(COMMIT_INFO_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.h")
set(COMMIT_INFO_SRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.c")
commit_info_library(
    "PDM_commit_info"
    "${COMMIT_INFO_HEADER_PATH}"
    "${COMMIT_INFO_SRC_PATH}"
    "${ARM_CORE}"
)

if("${TARGET}" STREQUAL "arm")
    jsoncan_library(
            "PDM_can"
            "PDM"
            "${CMAKE_CURRENT_BINARY_DIR}"
            TRUE
    )
    embedded_library(
            PDM_can
            "${CAN_SRCS}"
            "${CAN_INCLUDE_DIRS}"
            "${ARM_CORE}"
            TRUE
    )

    generate_stm32cube_code(
        "PDM"
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
        "stm32f4xx_hal_spi.c"
        "stm32f4xx_hal_tim_ex.c"
        "stm32f4xx_hal_tim.c"
        "stm32f4xx_hal.c"
        "stm32f4xx_ll_adc.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/Src/syscalls.c")
    stm32f412rx_cube_library(
        "PDM_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${SHARED_HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${SHARED_HW_INCLUDE_DIR})
    embedded_binary(
        "PDM.elf"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )
    target_link_libraries("PDM_can" "PDM_stm32cube")
    target_link_libraries("PDM.elf" "PDM_stm32cube" "PDM_can" "PDM_commit_info")

elseif("${TARGET}" STREQUAL "x86")
    jsoncan_library(
            "PDM_can"
            "PDM"
            "${CMAKE_CURRENT_BINARY_DIR}"
            FALSE
    )
    add_library(PDM_can STATIC "${CAN_SRCS}")
    target_include_directories(PDM_can PUBLIC "${CAN_INCLUDE_DIRS}")

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${FFF_DIR})
    compile_gtest_executable(
        "PDM_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("PDM_test" "PDM_can" "shared_fakes")
endif()
