message("")
message("⚙️ Configuring PCM (Quintuna)")

file(GLOB CUBEMX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

file(GLOB SYSTEM_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
set(SYSTEM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/boot")

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.c")
list(APPEND APP_SRCS
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.c")
list(APPEND IO_SRCS
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB_RECURSE HW_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.c")
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR}/hw_error.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_ubsan.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_i2c.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_utils.c"
        "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR}"
)

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm4")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app_only.ld")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})
    embedded_binary(
            "quintuna_PCM_app"
            "${SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
    )

    set(STM32_HAL_SRCS
            "stm32f4xx_hal_cortex.c"
            "stm32f4xx_hal_dma.c"
            "stm32f4xx_hal_exti.c"
            "stm32f4xx_hal_gpio.c"
            "stm32f4xx_hal_pwr_ex.c"
            "stm32f4xx_hal_rcc_ex.c"
            "stm32f4xx_hal_rcc.c"
            "stm32f4xx_hal_i2c.c"
            "stm32f4xx_hal.c"
    )

    stm32f412rx_cube_library(
            "quintuna_PCM_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/PCM.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDE_DIRS}"
            FALSE
            "${ARM_CORE}"
    )
    embedded_no_checks("${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c")

    target_link_libraries("quintuna_PCM_app.elf" PRIVATE "quintuna_PCM_stm32")
endif ()