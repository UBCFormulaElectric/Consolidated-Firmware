file(GLOB_RECURSE ARM_BINARY_APP_SRCS "Src/App/*.*")
set(ARM_BINARY_APP_INCLUDE_DIRS "Inc/App")
file(GLOB ARM_BINARY_IO_SRCS
        "Src/*.c"
        "Src/Io/*.c"
        "startup/*.s"
        )
set(ARM_BINARY_IO_INCLUDE_DIRS
        "Inc"
        "Inc/Io"
        )
set(ARM_BINARY_IO_SYSCALLS_SRC "Src/syscalls.c")
set(ARM_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32F302RCTx_FLASH.ld")

set(ARM_BINARY_IO_THIRD_PARTY_INCLUDE_DIRS
        "Drivers/STM32F3xx_HAL_Driver/Inc"
        "Drivers/STM32F3xx_HAL_Driver/Inc/Legacy"
        "Middlewares/Third_Party/FreeRTOS/Source/include"
        "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS"
        "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
        "Drivers/CMSIS/Device/ST/STM32F3xx/Include"
        "Drivers/CMSIS/Include"
        )
file(GLOB_RECURSE ARM_BINARY_IO_THIRD_PARTY_SRCS
        "Middlewares/*.c"
        "Drivers/*.c"
        ${ARM_BINARY_IO_SYSCALLS_SRC}
        )
set(IOC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/PDM.ioc")
set(STM32_DEVICE_TYPE STM32F302xC)

file(GLOB GOOGLETEST_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/*.cpp")
list(REMOVE_ITEM GOOGLETEST_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/main.cpp")
set(GOOGLETEST_TEST_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc")

create_arm_binary_or_tests_for_board(
    "PDM"
    "${IOC_FILE}"
    "${ARM_BINARY_APP_SRCS}"
    "${ARM_BINARY_APP_INCLUDE_DIRS}"
    "${ARM_BINARY_IO_SRCS}"
    "${ARM_BINARY_IO_INCLUDE_DIRS}"
    "${ARM_LINKER_SCRIPT}"
    "${STM32_DEVICE_TYPE}"
    "${ARM_BINARY_IO_THIRD_PARTY_SRCS}"
    "${ARM_BINARY_IO_THIRD_PARTY_INCLUDE_DIRS}"
    "${GOOGLETEST_TEST_SRCS}"
    "${GOOGLETEST_TEST_INCLUDE_DIRS}"
)
