set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DIM.ioc")
file(GLOB_RECURSE APP_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c"
    "${CMAKE_SOURCE_DIR}/firmware/shared/src/app/*.c"
)
set(APP_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
    "${CMAKE_SOURCE_DIR}/firmware/shared/src/app"    
)
file(GLOB_RECURSE IO_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c"    
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Startup/*.s"
    "${CMAKE_SOURCE_DIR}/firmware/shared/src/io/*.c"    
    "${CMAKE_SOURCE_DIR}/firmware/shared/src/hw/*.c"
)
set(IO_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
    "${CMAKE_SOURCE_DIR}/firmware/shared/src/io"    
    "${CMAKE_SOURCE_DIR}/firmware/shared/src/hw"    
)
file(GLOB_RECURSE TEST_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
    "${CMAKE_SOURCE_DIR}/firmware/shared/test_utils/*.*"
)
set(TEST_INCLUDE_DIRS 
    "${CMAKE_CURRENT_SOURCE_DIR}/test"
    "${CMAKE_SOURCE_DIR}/firmware/shared/test_utils"
)

if("${PLATFORM}" STREQUAL "arm")
    generate_stm32cube_code(
        "DIM"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        stm32f4xx_hal_adc_ex.c
        stm32f4xx_hal_adc.c
        stm32f4xx_hal_can.c
        stm32f4xx_hal_cortex.c
        stm32f4xx_hal_dma_ex.c
        stm32f4xx_hal_dma.c
        stm32f4xx_hal_exti.c
        stm32f4xx_hal_flash_ex.c
        stm32f4xx_hal_flash_ramfunc.c
        stm32f4xx_hal_flash.c
        stm32f4xx_hal_gpio.c
        stm32f4xx_hal_iwdg.c
        stm32f4xx_hal_pwr_ex.c
        stm32f4xx_hal_pwr.c
        stm32f4xx_hal_rcc_ex.c
        stm32f4xx_hal_rcc.c
        stm32f4xx_hal_spi.c
        stm32f4xx_hal_tim_ex.c
        stm32f4xx_hal_tim.c
        stm32f4xx_hal.c
        stm32f4xx_ll_adc.c
    )
    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32f4_cube_library(
        "DIM_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
        "STM32F412Rx"
    )

    jsoncan_library(
        "DIM_can"
        "DIM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        "STM32F412Rx"
    )
    target_link_libraries("DIM_can" "DIM_stm32cube")

    set(SRCS ${APP_SRCS} ${IO_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS})
    set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/STM32F412RGTX_FLASH.ld)
    compile_embedded_binary(
        "DIM.elf"
        "${SRCS}"
        "${INCLUDE_DIRS}"
        "STM32F412Rx"
        "${LINKER_SCRIPT}"
    )
    target_link_libraries("DIM.elf" "DIM_stm32cube" "DIM_can")

elseif("${PLATFORM}" STREQUAL "x86")
    jsoncan_library(
        "DIM_can"
        "DIM"
        "${CMAKE_CURRENT_BINARY_DIR}/jsoncan"
        ""
    )

    set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sevenSegDisplays.h"
    )
    create_fake_library(
        "DIM_fakes"
        "${HEADERS_TO_FAKE}"
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