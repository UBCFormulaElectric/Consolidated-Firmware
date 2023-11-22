set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/f4boot.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_boot.ld")

list(APPEND APP_SRCS "${SHARED_APP_SRCS}")
set(APP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/app" "${SHARED_APP_INCLUDE_DIR}")
list(REMOVE_ITEM APP_SRCS "${SHARED_APP_INCLUDE_DIR}/App_SharedSetPeriodicCanSignals.c")
list(REMOVE_ITEM APP_SRCS "${SHARED_APP_INCLUDE_DIR}/App_SharedStateMachine.c")
list(REMOVE_ITEM APP_SRCS "${SHARED_APP_INCLUDE_DIR}/App_SharedWaitSignal.c")
list(REMOVE_ITEM APP_SRCS "${SHARED_APP_INCLUDE_DIR}/App_SharedSignal.c")

list(APPEND IO_SRCS "${SHARED_IO_SRCS}")
list(REMOVE_ITEM IO_SRCS "${SHARED_IO_INCLUDE_DIR}/Io_SharedSpi.c")
list(REMOVE_ITEM IO_SRCS "${SHARED_IO_INCLUDE_DIR}/Io_SharedFreqOnlyPwmInput.c")
list(REMOVE_ITEM IO_SRCS "${SHARED_IO_INCLUDE_DIR}/Io_SharedPwmInput.c")
list(REMOVE_ITEM IO_SRCS "${SHARED_IO_INCLUDE_DIR}/Io_SharedAdc.c")
list(REMOVE_ITEM IO_SRCS "${SHARED_IO_INCLUDE_DIR}/io_jsoncan.c")
set(IO_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/io" "${SHARED_IO_INCLUDE_DIR}")

file(GLOB HW_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/Src/*.c"
    "${BOOT_DIR}/*.c"
)
list(APPEND HW_SRCS "${SHARED_HW_SRCS}")
list(REMOVE_ITEM HW_SRCS "${SHARED_HW_INCLUDE_DIR}/hw_sd.c")
set(HW_INCLUDE_DIRS "${SHARED_HW_INCLUDE_DIR}" "${BOOT_DIR}")

if("${TARGET}" STREQUAL "deploy")
    generate_stm32cube_code(
        "f4boot"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        "stm32f4xx_hal_adc_ex.c"
        "stm32f4xx_hal_adc.c"
        "stm32f4xx_hal_can.c" 
        "stm32f4xx_hal_crc.c" 
        "stm32f4xx_hal_cortex.c"
        "stm32f4xx_hal_dma_ex.c"
        "stm32f4xx_hal_dma.c"
        "stm32f4xx_hal_exti.c"
        "stm32f4xx_hal_flash.c"
        "stm32f4xx_hal_flash_ex.c"
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
    set(SYSCALLS "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/Src/syscalls.c")
    stm32f412rx_cube_library(
        "f4boot_stm32cube"
        "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/Inc"
        "${STM32_HAL_SRCS}"
        "${SYSCALLS}"
        "${IOC_PATH}.md5"
    )

    set(SRCS ${APP_SRCS} ${IO_SRCS} ${HW_SRCS})
    set(INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${IO_INCLUDE_DIRS} ${HW_INCLUDE_DIRS})

    function(stm32f4rx_bootloader NAME BOARD_DEFINE BOARD_SPECIFIC_SRC)
        set(BOARD_SPECIFIC_SRCS ${SRCS} ${BOARD_SPECIFIC_SRC})
        embedded_binary(
            "${NAME}"
            "${BOARD_SPECIFIC_SRCS}"
            "${INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
        )
        target_link_libraries("${NAME}.elf" "f4boot_stm32cube")
        target_compile_definitions("${NAME}.elf" PRIVATE "${BOARD_DEFINE}")
    endfunction()

    stm32f4rx_bootloader("BMS_boot" "BMS" "")
    stm32f4rx_bootloader("DCM_boot" "DCM" "")
    stm32f4rx_bootloader("FSM_boot" "FSM" "")
    stm32f4rx_bootloader("PDM_boot" "PDM" "${CMAKE_CURRENT_SOURCE_DIR}/boot_PDM.c")
    stm32f4rx_bootloader("DIM_boot" "DIM" "")
    stm32f4rx_bootloader("f4dev_boot" "f4dev" "")
endif()
