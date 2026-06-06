message("")
message("⚙️ Configuring PCM (Hexray)")

file(GLOB CUBEMX_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c")
set(CUBEMX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc")

file(GLOB SYSTEM_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

file(GLOB_RECURSE APP_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.cpp")
list(APPEND APP_SRCS
)
set(APP_INCLUDE_DIRS
        #         "${CMAKE_CURRENT_SOURCE_DIR}/src/app"
        "${SHARED_APP_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE IO_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
list(APPEND IO_SRCS
)
set(IO_INCLUDE_DIRS
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/io"
        "${SHARED_IO_INCLUDE_DIR_CPP}"
)

file(GLOB_RECURSE HW_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.cpp")
list(APPEND HW_SRCS
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_error.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_i2c.cpp"
        #        "${SHARED_HW_INCLUDE_DIR}/hw_utils.cpp"
        "${SHARED_HW_INCLUDE_DIR_CPP}/hw_hardFaultHandler.cpp"
)
set(HW_INCLUDE_DIRS
        # "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR_CPP}"
)

if ("${TARGET}" STREQUAL "binary")
    set(ARM_CORE "cm4")
    set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app_only.ld")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SHARED_UTIL_INCLUDE_DIR} ${SYSTEM_INCLUDE_DIRS} ${CUBEMX_INCLUDE_DIRS})
    embedded_binary(
            "hexray_PCM_app"
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
    set(CUBEMX_INCLUDES ${CUBEMX_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})
    stm32f412rx_cube_library(
            "hexray_PCM_stm32"
            "${STM32_HAL_SRCS}"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/PCM.ioc"
            "${CUBEMX_SRCS}"
            "${CUBEMX_INCLUDES}"
            FALSE
            "${ARM_CORE}"
    )

    target_link_libraries("hexray_PCM_app.elf" PRIVATE "hexray_PCM_stm32")
    target_compile_definitions("hexray_PCM_app.elf" PRIVATE NO_BOOTLOADER)
endif ()