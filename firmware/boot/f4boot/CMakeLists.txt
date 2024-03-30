set(ARM_CORE "cm4")
set(IOC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/f4boot.ioc")
set(LINKER_SCRIPT "${LINKER_DIR}/stm32f412rgtx/stm32f412rgtx_boot.ld")

file(GLOB BOOT_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/cubemx/Src/*.c"
    "${BOOT_DIR}/*.c"
)
list (APPEND BOOT_SRCS 
    "${SHARED_IO_INCLUDE_DIR}/io_can.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_flash.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_can.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_gpio.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_hardFaultHandler.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_crc.c"
    "${SHARED_HW_INCLUDE_DIR}/hw_assert.c"
)
set(BOOT_INCLUDE_DIRS 
    "${BOOT_DIR}"
    "${SHARED_APP_INCLUDE_DIR}" 
    "${SHARED_IO_INCLUDE_DIR}" 
    "${SHARED_HW_INCLUDE_DIR}" 
)

if("${TARGET}" STREQUAL "deploy")
    generate_stm32cube_code(
        "f4boot"
        "${IOC_PATH}"
    )

    set(STM32_HAL_SRCS
        "stm32f4xx_hal_can.c" 
        "stm32f4xx_hal_crc.c" 
        "stm32f4xx_hal_cortex.c"
        "stm32f4xx_hal_exti.c"
        "stm32f4xx_hal_flash.c"
        "stm32f4xx_hal_flash_ex.c"
        "stm32f4xx_hal_gpio.c"
        "stm32f4xx_hal_rcc_ex.c"
        "stm32f4xx_hal_rcc.c"
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

    function(stm32f4_bootloader NAME BOARD_DEFINE BOARD_SPECIFIC_SRC)
        set(BOARD_SPECIFIC_SRCS ${BOOT_SRCS} ${BOARD_SPECIFIC_SRC} "${CMAKE_CURRENT_SOURCE_DIR}/bootloader_f4.c")
        embedded_binary(
            "${NAME}"
            "${BOARD_SPECIFIC_SRCS}"
            "${BOOT_INCLUDE_DIRS}"
            "${LINKER_SCRIPT}"
            "${ARM_CORE}"
        )
        target_link_libraries("${NAME}.elf" "f4boot_stm32cube")
        target_compile_definitions("${NAME}.elf" PRIVATE "${BOARD_DEFINE}")
    endfunction()

    stm32f4_bootloader("thruna_BMS_boot" "BMS" "")
    stm32f4_bootloader("thruna_DCM_boot" "DCM" "")
    stm32f4_bootloader("thruna_FSM_boot" "FSM" "")
    stm32f4_bootloader("thruna_PDM_boot" "PDM" "${CMAKE_CURRENT_SOURCE_DIR}/bootloader_PDM.c")
    stm32f4_bootloader("thruna_DIM_boot" "DIM" "")

    stm32f4_bootloader("quadruna_FSM_boot" "quadruna_FSM" "")
    stm32f4_bootloader("quadruna_RSM_boot" "quadruna_RSM" "")
    stm32f4_bootloader("quadruna_CRIT_boot" "quadruna_CRIT" "")
endif()
