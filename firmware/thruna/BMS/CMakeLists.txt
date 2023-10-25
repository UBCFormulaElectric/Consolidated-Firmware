set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/BMS.ioc")
set(LINKER_SCRIPT "${THIRD_PARTY_DIR}/linker/STM32F412RGTX_FLASH.ld")

file(GLOB_RECURSE APP_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/App/*.c"
)
list(APPEND APP_SRCS "${SHARED_APP_SRCS}")
set(APP_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc/App"
)   
list(APPEND APP_INCLUDE_DIRS "${SHARED_APP_INCLUDE_DIRS}")

file(GLOB_RECURSE IO_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Io/*.c"    
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c"
)
list(APPEND IO_SRCS "${SHARED_IO_SRCS}" "${SHARED_HW_SRCS}")
set(IO_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc/Io"
    "${CMAKE_CURRENT_SOURCE_DIR}/Inc"  
)
list(APPEND IO_INCLUDE_DIRS "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE TEST_SRCS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/*.cpp"
)
list(APPEND TEST_SRCS "${SHARED_TEST_UTILS_SRCS}")
set(TEST_INCLUDE_DIRS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc"
)
list(APPEND TEST_INCLUDE_DIRS "${SHARED_TEST_UTILS_INCLUDE_DIRS}")

if("${PLATFORM}" STREQUAL "arm")
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

    jsoncan_library(
        "BMS_can"
        "BMS"
        "${CMAKE_CURRENT_BINARY_DIR}"
    )
    target_link_libraries("BMS_can" "BMS_stm32cube")

    set(EMBEDDED_SRCS ${APP_SRCS} ${IO_SRCS})
    set(EMBEDDED_INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS})
    cm4_binary(
        "BMS.elf"
        "${EMBEDDED_SRCS}"
        "${EMBEDDED_INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
    )
    target_link_libraries("BMS.elf" "BMS_stm32cube" "BMS_can")

elseif("${PLATFORM}" STREQUAL "x86")
    jsoncan_library(
        "BMS_can"
        "BMS"
        "${CMAKE_CURRENT_BINARY_DIR}"
        ""
    )

    set(SRCS ${APP_SRCS} ${TEST_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${TEST_INCLUDE_DIRS} ${FFF_DIR})
    compile_gtest_executable(
        "BMS_test"
        "${SRCS}"
        "${INCLUDE_DIRS}"
    )
    target_link_libraries("BMS_test" "BMS_can" "shared_fakes")
endif()
