file(GLOB ARM_BINARY_APP_SOURCES
     "Src/App/*.c"
     "Src/App/StateMachine/*.c"
     )
set(ARM_BINARY_APP_INCLUDE_DIRS
    "Inc/App"
    )
file(GLOB ARM_BINARY_IO_SOURCES
     "Src/*.c"
     "Src/Io/*.c"
     "startup/*.s"
     )
set(ARM_BINARY_IO_INCLUDE_DIRS
    "Inc/Io"
    "Inc"
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
    "Middlewares/*.*"
    "Drivers/*.*" 
    )

set(ARM_SYSCALLS_SRC "Src/syscalls.c")
set(IOC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/DCM.ioc")
set(ARM_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32F302C8Tx_FLASH.ld")

create_arm_binary_or_tests_for_board(
    "DCM"
    "${IOC_FILE}"
    "${ARM_BINARY_APP_SOURCES}"
    "${ARM_BINARY_APP_INCLUDE_DIRS}"
    "${ARM_BINARY_IO_SOURCES}"
    "${ARM_BINARY_IO_INCLUDE_DIRS}"
    "${STM32CUBEMX_DRIVER_SRCS}"
    "${STM32CUBEMX_DRIVER_INCLUDE_DIRS}"
    "${ARM_LINKER_SCRIPT}"
    "${ARM_SYSCALLS_SRC}"
    )
