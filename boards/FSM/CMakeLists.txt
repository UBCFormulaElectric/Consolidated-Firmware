file(GLOB_RECURSE ARM_BINARY_SOURCES "Src/*.*")
set(ARM_BINARY_INCLUDE_DIRS 
    "Inc/"
    "Inc/States/"
    )
set(STM32CUBEMX_DRIVER_INCLUDE_DIRS
    "Drivers/STM32F3xx_HAL_Driver/Inc"
    "Drivers/STM32F3xx_HAL_Driver/Inc/Legacy"
    "Middlewares/Third_Party/FreeRTOS/Source/include"
    "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS"
    "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
    "Drivers/CMSIS/Device/ST/STM32F3xx/Include"
    "Drivers/CMSIS/Include"
    )
file(GLOB_RECURSE STM32CUBEMX_DRIVER_SRCS 
    "startup/*.*" 
    "Middlewares/*.*" 
    "Drivers/*.*" 
    "Src/syscalls.c"
    )
SET(IOC_FILE "${CMAKE_SOURCE_DIR}/FSM.ioc")
SET(LINKER_SCRIPT STM32F302C8Tx_FLASH.ld)

create_arm_binary_or_tests_for_board(
    "FSM"
    "${IOC_FILE}"
    "${ARM_BINARY_SOURCES}"
    "${ARM_BINARY_INCLUDE_DIRS}"
    "${STM32CUBEMX_DRIVER_SRCS}"
    "${STM32CUBEMX_DRIVER_INCLUDE_DIRS}"
    "${LINKER_SCRIPT}"
    )

