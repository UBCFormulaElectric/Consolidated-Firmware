set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/f4dev.ioc")
set(SHARED_DIR "${FIRMWARE_DIR}/shared")
set(LINKER_SCRIPT "${THIRD_PARTY_DIR}/linker/STM32F412RGTX_FLASH.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS "${SHARED_APP_SRCS}")
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

# file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
# list(APPEND IO_SRCS "${SHARED_IO_SRCS}")
# list(REMOVE_ITEM IO_SRCS "${SHARED_IO_INCLUDE_DIR}/Io_SharedSpi.c")
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)

list (APPEND HW_SRCS "${SHARED_DIR}/src/hw/hw_sdio_sd.c")
# list(APPEND IO_SRCS "${SHARED_HW_SRCS}")
set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${SHARED_HW_INCLUDE_DIR}")

# file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/*.cpp")
# set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc")
# list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

# Autogenerate jsoncan library.
# jsoncan_library(
#     "f4dev_can"
#     "f4dev"
#     "${CMAKE_CURRENT_BINARY_DIR}"
#     "${ARM_CORE}"
# )

# Autogenerate git commit info header/source.
set(COMMIT_INFO_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.h")
set(COMMIT_INFO_SRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.c")
commit_info_library(
    "f4dev_commit_info"
    "${COMMIT_INFO_HEADER_PATH}"
    "${COMMIT_INFO_SRC_PATH}"
    "${ARM_CORE}"
)

if("${PLATFORM}" STREQUAL "arm")
    generate_stm32cube_code(
        "f4dev"
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
        "stm32f4xx_hal_sd.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32f412rx_cube_library(
        "f4dev_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS  ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    embedded_binary(
        "f4dev.elf"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )
    target_link_libraries("f4dev.elf" "f4dev_stm32cube" "f4dev_commit_info" "littlefs_cm4")

# elseif("${PLATFORM}" STREQUAL "x86")
#     set(HEADERS_TO_FAKE
#         "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sevenSegDisplays.h"
#     )
#     create_fake_library(
#         "f4dev_fakes"
#         "${HEADERS_TO_FAKE}"
#     )

#     set(SRCS ${APP_SRCS} ${TEST_SRCS})
#     set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
#     compile_gtest_executable(
#         "f4dev_test"
#         "${SRCS}"
#         "${INCLUDE_DIRS}"
#     )
#     target_link_libraries("f4dev_test" "f4dev_can" "f4dev_fakes" "f4dev_commit_info" "shared_fakes")
endif()
