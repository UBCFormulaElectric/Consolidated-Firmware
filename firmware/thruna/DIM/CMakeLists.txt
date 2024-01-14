set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DIM.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS "${SHARED_APP_SRCS}")
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS "${SHARED_IO_SRCS}")
list(REMOVE_ITEM IO_SRCS "${SHARED_IO_INCLUDE_DIR}/Io_SharedSpi.c")
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
list(APPEND HW_SRCS "${SHARED_HW_SRCS}")
list(REMOVE_ITEM HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_sd.c")
list(REMOVE_ITEM HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_crc.c")
list(REMOVE_ITEM HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_flash.c")
list(REMOVE_ITEM HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_fdcan.c")

set(HW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw" "${SHARED_HW_INCLUDE_DIR}")

file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/*.cpp")
set(TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc")
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

if("${TARGET}" STREQUAL "deploy")
    jsoncan_library(
        "DIM_can"
        "DIM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        TRUE
    )
    embedded_library(
        DIM_can
        "${CAN_SRCS}"
        "${CAN_INCLUDE_DIRS}"
        "${ARM_CORE}"
        TRUE
    )

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

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})
    embedded_binary(
        "DIM_app"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )

    # Autogenerate git commit info header/source.
    set(COMMIT_INFO_HEADER_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.h")
    set(COMMIT_INFO_SRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/app/App_CommitInfo.c")
    commit_info_library(
        "DIM_app.elf"
        "DIM_commit_info"
        "${COMMIT_INFO_HEADER_PATH}"
        "${COMMIT_INFO_SRC_PATH}"
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
