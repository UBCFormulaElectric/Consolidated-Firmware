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
SET(IOC_FILE "${CMAKE_SOURCE_DIR}/PDM.ioc")

board_specific_code_generation(
    "PDM"
    "${IOC_FILE}"
    "Inc/git_hash.h"
    "${ARM_BINARY_SOURCES}"
    "${ARM_BINARY_INCLUDE_DIRS}"
    "${STM32CUBEMX_DRIVER_SRCS}"
    "${STM32CUBEMX_DRIVER_INCLUDE_DIRS}"
    )

