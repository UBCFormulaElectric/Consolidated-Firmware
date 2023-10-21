if("${PLATFORM}" STREQUAL "arm")
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
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32cube_library(
        "dim_stm32cube"
        "${CMAKE_SOURCE_DIR}/firmware/third_party/STM32CubeF4"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "STM32F412Rx"
    )

    file(GLOB_RECURSE NEWLIB_FREERTOS_PATCH_SRCS ${CMAKE_SOURCE_DIR}/firmware/third_party/newlib_freertos_patch/*.c)
    compile_embedded_lib(
        "dim_newlib"
        "${NEWLIB_FREERTOS_PATCH_SRCS}"
        ""
        ""
    )
    target_link_libraries("dim_newlib" "dim_stm32cube")

    jsoncan_library(
        "dim_can"
        "DIM"
        "${CMAKE_CURRENT_BINARY_DIR}"
        "STM32F412Rx"
    )
    target_link_libraries("dim_can" "dim_stm32cube")

    file(GLOB_RECURSE TARGET_EMBEDDED_SRCS 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c"    
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Startup/*.s"
    )
    list(REMOVE_ITEM TARGET_EMBEDDED_SRCS "${SYSCALLS}")
    set(TARGET_EMBEDDED_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
    )
    set(SHARED_EMBEDDED_INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/firmware/shared/src/app"
        "${CMAKE_SOURCE_DIR}/firmware/shared/src/io"
        "${CMAKE_SOURCE_DIR}/firmware/shared/src/hw"
    )
    file(GLOB_RECURSE SHARED_EMBEDDED_SRCS 
        "${CMAKE_SOURCE_DIR}/firmware/shared/src/app/*.c"
        "${CMAKE_SOURCE_DIR}/firmware/shared/src/io/*.c"
        "${CMAKE_SOURCE_DIR}/firmware/shared/src/hw/*.c"
    )
    set(EMBEDDED_SRCS ${TARGET_EMBEDDED_SRCS} ${SHARED_EMBEDDED_SRCS})
    set(EMBEDDED_INCLUDE_DIRS ${TARGET_EMBEDDED_INCLUDE_DIRS} ${SHARED_EMBEDDED_INCLUDE_DIRS})

    list(APPEND EMBEDDED_SRCS "${CMAKE_SOURCE_DIR}/firmware/third_party/SEGGER_SystemView_Src/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c")

    set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/STM32F412RGTX_FLASH.ld)
    compile_embedded_binary(
        "dim.elf"
        "${EMBEDDED_SRCS}"
        "${EMBEDDED_INCLUDE_DIRS}"
        "STM32F412Rx"
        "${LINKER_SCRIPT}"
    )
    target_link_libraries("dim.elf" "dim_stm32cube" "dim_can" "dim_newlib")
endif()

if("${PLATFORM}" STREQUAL "x86")
    set(TARGET_TEST_INCLUDE_DIRS 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${CMAKE_CURRENT_SOURCE_DIR}/test"
    )
    file(GLOB_RECURSE TARGET_TEST_SRCS 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
    )
    set(SHARED_TEST_INCLUDE_DIRS 
        "${CMAKE_SOURCE_DIR}/firmware/shared/src/app"
        "${CMAKE_SOURCE_DIR}/firmware/shared/src/io"
        "${CMAKE_SOURCE_DIR}/firmware/shared/test_utils"
    )
    file(GLOB_RECURSE SHARED_TEST_SRCS "${CMAKE_SOURCE_DIR}/firmware/shared/src/app/*.c")
    set(TEST_SRCS ${TARGET_TEST_SRCS} ${SHARED_TEST_SRCS})
    set(TEST_INCLUDE_DIRS ${TARGET_TEST_INCLUDE_DIRS} ${SHARED_TEST_INCLUDE_DIRS})

    jsoncan_library(
        "dim_can"
        "DIM"
        "${CMAKE_CURRENT_BINARY_DIR}/jsoncan"
        ""
    )

    set(HEADERS_TO_FAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sevenSegDisplays.h"
    )
    create_fake_library(
        "dim_fakes"
        "${HEADERS_TO_FAKE}"
    )

    compile_gtest_executable(
        "dim_test"
        "${TEST_SRCS}"
        "${TEST_INCLUDE_DIRS}"
    )
    target_link_libraries("dim_test" "dim_can" "dim_fakes" "shared_fakes")
endif()