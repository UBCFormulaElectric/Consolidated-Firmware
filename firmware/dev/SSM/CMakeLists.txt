message("")
message("⚙️ Configuring SSM (Quintuna)")
set(ARM_CORE "cm4")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_app.ld")

file(GLOB_RECURSE SYSTEM_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/*.c"
)
set(SYSTEM_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/app/*.cpp")
list(APPEND APP_SRCS
        ""
)
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/app" "${SHARED_APP_INCLUDE_DIR_CPP}")

file(GLOB_RECURSE IO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/io/*.cpp")
list(APPEND IO_SRCS
        ""
)
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/io" "${SHARED_IO_INCLUDE_DIR_CPP}")

file(GLOB_RECURSE HW_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw/*.cpp"
)
list(APPEND HW_SRCS
	""
)
set(HW_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/hw"
        "${SHARED_HW_INCLUDE_DIR_CPP}"
)

if ("${TARGET}" STREQUAL "binary")
    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS} ${TASKS_SRCS} ${SYSTEM_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS})

        # Bootloader files.
        file(GLOB_RECURSE BOOT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/boot/*.c")
        set(BOOT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/boot" "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Inc")
        stm32f4_boot_binary(
                "quintuna_SSM_boot" 
                "${BOOT_SRCS}"
                "${BOOT_INCLUDE_DIRS}"
                "quintuna_SSM"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/Src/syscalls.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/boot/cubemx/SSM_boot.c"
        )
        set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/SSM.ioc")
        generate_stm32cube_code("SSM" "${IOC_PATH}")
        set(STM32_HAL_SRCS
                "stm32f4xx_hal_adc_ex.c"
                "stm32f4xx_hal_adc.c"
                "stm32f4xx_hal_cortex.c"
                "stm32f4xx_hal_dma_ex.c"
                "stm32f4xx_hal_dma.c"
                "stm32f4xx_hal_exti.c"
                "stm32f4xx_hal_can.c"
                "stm32f4xx_hal_gpio.c"
                "stm32f4xx_hal_iwdg.c"
                "stm32f4xx_hal_pwr_ex.c"
                "stm32f4xx_hal_rcc_ex.c"
                "stm32f4xx_hal_rcc.c"
                "stm32f4xx_hal_tim_ex.c"
                "stm32f4xx_hal_tim.c"
                "stm32f4xx_hal_uart.c"
                "stm32f4xx_hal.c"
        )

   stm32f412rx_cube_library(
        "quintuna_SSM_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Src/syscalls.c"
        "${MD5_LOCATION}"
    )

    embedded_binary(
        "quintuna_SSM_app"
        "${SRCS}"
        "${INCLUDE_DIRECTORIES}"
        "${LINKER_SCRIPT}"
        "${ARM_CORE}"
    )

     # JSONCAN
     jsoncan_embedded_library(SSM quintuna "${CMAKE_CURRENT_BINARY_DIR}" ${ARM_CORE})

     # CommitInfo
     commit_info_library(
        "quintuna_SSM_app.elf"
        "quintuna_SSM_commit_info"
        "${CMAKE_CURRENT_BINARY_DIR}/app"
        "${ARM_CORE}"
     )

     target_link_libraries("quintuna_SSM_jsoncan" PRIVATE "quintuna_SSM_stm32cube")
     target_link_libraries("quintuna_SSM_app.elf" PRIVATE "quintuna_SSM_stm32cube" "quintuna_SSM_jsoncan" "quintuna_SSM_commit_info" "debug_modules")
     target_link_libraries("quintuna_SSM_boot.elf" PRIVATE "quintuna_SSM_commit_info")

     embedded_image(
        "quintuna_SSM"
        "quintuna_SSM_app.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/quintuna_SSM_app.hex"
        "quintuna_SSM_boot.hex"
        "${CMAKE_CURRENT_BINARY_DIR}/quintuna_SSM_boot.hex"
)

elseif ("${TARGET}" STREQUAL "test")
endif ()