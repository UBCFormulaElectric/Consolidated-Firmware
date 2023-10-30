set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/FSM.ioc")
set(LINKER_SCRIPT "${THIRD_PARTY_DIR}/linker/STM32F412RGTX_FLASH.ld")

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

# Autogenerate jsoncan library.
jsoncan_library(
    "FSM_can"
    "FSM"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${ARM_CORE}"
)

# Autogenerate git commit info header/source.
set(COMMIT_INFO_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.h")
set(COMMIT_INFO_SRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.c")
commit_info_library(
    "FSM_commit_info"
    "${COMMIT_INFO_HEADER_PATH}"
    "${COMMIT_INFO_SRC_PATH}"
    "${ARM_CORE}"
)

if("${PLATFORM}" STREQUAL "arm")
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
        "stm32f4xx_hal_spi.c"
        "stm32f4xx_hal_tim_ex.c"
        "stm32f4xx_hal_tim.c"
        "stm32f4xx_hal.c"
        "stm32f4xx_ll_adc.c"
    )
    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/Src/syscalls.c")
    stm32f412rx_cube_library(
        "FSM_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${SHARED_HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${SHARED_HW_INCLUDE_DIR})
    embedded_binary(
        "FSM.elf"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )
    target_link_libraries("FSM_can" "FSM_stm32cube")
    target_link_libraries("FSM.elf" "FSM_stm32cube" "FSM_can" "FSM_commit_info")

elseif("${PLATFORM}" STREQUAL "x86")
    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${FFF_DIR})
    compile_gtest_executable(
        "FSM_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("FSM_test" "FSM_can" "shared_fakes")
endif()
