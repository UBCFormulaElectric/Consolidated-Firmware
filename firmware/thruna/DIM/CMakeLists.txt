set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DIM.ioc")
set(LINKER_SCRIPT "${THIRD_PARTY_DIR}/linker/STM32F412RGTX_FLASH.ld")

file(GLOB_RECURSE APP_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c"
)
list(APPEND APP_SRCS "${SHARED_APP_SRCS}")
set(APP_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
)
list(APPEND APP_INCLUDE_DIRS "${SHARED_APP_INCLUDE_DIRS}")

file(GLOB_RECURSE IO_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
list(APPEND IO_SRCS "${SHARED_IO_SRCS}")
set(IO_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
)
list(APPEND IO_INCLUDE_DIRS "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE TEST_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
)
list(APPEND TEST_SRCS "${SHARED_TEST_UTILS_SRCS}")
set(TEST_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/test"
)
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

if("${PLATFORM}" STREQUAL "arm")
    generate_stm32cube_code(
        "DIM"
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

    jsoncan_library(
        "DIM_can"
        "DIM"
        "${CMAKE_CURRENT_BINARY_DIR}"
    )
    target_link_libraries("DIM_can" "DIM_stm32cube")

    set(EMBEDDED_SRCS ${APP_SRCS} ${IO_SRCS})
    set(EMBEDDED_INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS})
    cm4_binary(
        "DIM.elf"
        "${EMBEDDED_SRCS}"
        "${EMBEDDED_INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
    )
    target_link_libraries("DIM.elf" "DIM_stm32cube" "DIM_can")

elseif("${PLATFORM}" STREQUAL "x86")
    jsoncan_library(
        "DIM_can"
        "DIM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        ""
    )

    set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sevenSegDisplays.h"
    )
    create_fake_library(
        "DIM_fakes"
        "${HEADERS_TO_FAKE}"
        "${SHARED_APP_INCLUDE_DIRS}"
    )

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
    compile_gtest_executable(
        "DIM_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("DIM_test" "DIM_can" "DIM_fakes" "shared_fakes")
endif()
