file(GLOB_RECURSE ARM_BINARY_APP_SRCS "src/app/*.*")
set(ARM_BINARY_APP_INCLUDE_DIRS "src/app")
file(GLOB ARM_BINARY_IO_SRCS
    "src/dev/*.c"
    "src/hw/*.c"
    "src/stm32/Src/*.c"
    "src/stm32/startup/*.s"
)
set(ARM_BINARY_IO_INCLUDE_DIRS
    "src/dev"
    "src/hw"
    "src/stm32/Inc"
)

set(ARM_BINARY_X86_COMPATIBLE_SRCS
    ${ARM_BINARY_APP_SRCS})

list(REMOVE_ITEM ARM_BINARY_IO_SRCS "")
set(ARM_BINARY_X86_INCOMPATIBLE_SRCS ${ARM_BINARY_IO_SRCS})

set(ARM_BINARY_INCLUDE_DIRS
    ${ARM_BINARY_APP_INCLUDE_DIRS}
    ${ARM_BINARY_IO_INCLUDE_DIRS})

set(ARM_BINARY_IO_SYSCALLS_SRC "src/stm32/Src/syscalls.c")
list(APPEND ARM_BINARY_THIRD_PARTY_SRCS ${ARM_BINARY_IO_SYSCALLS_SRC})
set(ARM_LINKER_SCRIPT "src/stm32/STM32F412RGTx_FLASH.ld")

set(STM32CUBEMX_DRIVER_INCLUDE_DIRS
    "src/stm32/Drivers/STM32F4xx_HAL_Driver/Inc"
    "src/stm32/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy"
    "src/stm32/Middlewares/Third_Party/FreeRTOS/Source/include"
    "src/stm32/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS"
    "src/stm32/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
    "src/stm32/Drivers/CMSIS/Device/ST/STM32F4xx/Include"
    "src/stm32/Drivers/CMSIS/Include"
)
list(APPEND ARM_BINARY_THIRD_PARTY_INCLUDE_DIRS ${STM32CUBEMX_DRIVER_INCLUDE_DIRS})
file(GLOB_RECURSE STM32CUBEMX_DRIVER_SRCS
    "src/stm32/Middlewares/*.c"
    "src/stm32/Drivers/*.c"
)
list(APPEND ARM_BINARY_THIRD_PARTY_SRCS ${STM32CUBEMX_DRIVER_SRCS})

set(IOC_FILE "src/stm32/DIM.ioc")
set(STM32_DEVICE_TYPE STM32F412Rx)

file(GLOB GOOGLETEST_TEST_SRCS "test/*.cpp")
set(GOOGLETEST_TEST_INCLUDE_DIRS
    "test"
)

create_arm_binary_or_tests_for_board(
    "DIM"
    "${IOC_FILE}"
    "${ARM_BINARY_X86_COMPATIBLE_SRCS}"
    "${ARM_BINARY_X86_INCOMPATIBLE_SRCS}"
    "${ARM_BINARY_INCLUDE_DIRS}"
    "${ARM_BINARY_THIRD_PARTY_SRCS}"
    "${ARM_BINARY_THIRD_PARTY_INCLUDE_DIRS}"
    "${ARM_LINKER_SCRIPT}"
    "${STM32_DEVICE_TYPE}"
    "${GOOGLETEST_TEST_SRCS}"
    "${GOOGLETEST_TEST_INCLUDE_DIRS}"
)
