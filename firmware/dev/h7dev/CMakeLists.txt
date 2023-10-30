set(ARM_CORE "cm7")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/h7dev.ioc")
set(LINKER_SCRIPT "${THIRD_PARTY_DIR}/linker/STM32H733VGTX_FLASH.ld")

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

list(REMOVE_ITEM APP_SRCS "${SHARED_EMBEDDED_DIR}/app/App_SharedSetPeriodicCanSignals.c")
list(REMOVE_ITEM IO_SRCS "${SHARED_EMBEDDED_DIR}/io/Io_SharedCan.c")

if("${PLATFORM}" STREQUAL "arm")
    generate_stm32cube_code(
        "h7dev"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        "stm32h7xx_hal_adc_ex.c"
        "stm32h7xx_hal_adc.c"
        "stm32h7xx_hal_cortex.c"
        "stm32h7xx_hal_dma_ex.c"
        "stm32h7xx_hal_dma.c"
        "stm32h7xx_hal_exti.c"
        "stm32h7xx_hal_gpio.c"
        "stm32h7xx_hal_iwdg.c"
        "stm32h7xx_hal_pwr_ex.c"
        "stm32h7xx_hal_rcc_ex.c"
        "stm32h7xx_hal_rcc.c"
        "stm32h7xx_hal_spi.c"
        "stm32h7xx_hal_tim_ex.c"
        "stm32h7xx_hal_tim.c"
        "stm32h7xx_hal.c"
    )

    # Pass syscalls to the cube library so we can build without warnings.
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")
    stm32h733xx_cube_library(
        "h7dev_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(EMBEDDED_SRCS ${APP_SRCS} ${IO_SRCS})
    set(EMBEDDED_INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS})
    embedded_binary(
        "h7dev.elf"
        "${EMBEDDED_SRCS}"
        "${EMBEDDED_INCLUDE_DIRS}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )
    target_link_libraries("h7dev.elf" "h7dev_stm32cube")
endif()
