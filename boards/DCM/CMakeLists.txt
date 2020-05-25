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
list(APPEND ARM_BINARY_THIRD_PARTY_SRCS ${ARM_BINARY_IO_SYSCALLS_SRC})
set(ARM_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32F302CCTx_FLASH.ld")

set(STM32CUBEMX_DRIVER_INCLUDE_DIRS
        "Drivers/STM32F3xx_HAL_Driver/Inc"
        "Drivers/STM32F3xx_HAL_Driver/Inc/Legacy"
        "Middlewares/Third_Party/FreeRTOS/Source/include"
        "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS"
        "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
        "Drivers/CMSIS/Device/ST/STM32F3xx/Include"
        "Drivers/CMSIS/Include"
        )
list(APPEND ARM_BINARY_THIRD_PARTY_INCLUDE_DIRS ${STM32CUBEMX_DRIVER_INCLUDE_DIRS})
file(GLOB_RECURSE STM32CUBEMX_DRIVER_SRCS
        "Middlewares/*.c"
        "Drivers/*.c"
        )
list(APPEND ARM_BINARY_THIRD_PARTY_SRCS ${STM32CUBEMX_DRIVER_SRCS})

set(IOC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/DCM.ioc")
set(STM32_DEVICE_TYPE STM32F302xC)

file(GLOB GOOGLETEST_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/*.cpp")
list(REMOVE_ITEM GOOGLETEST_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/main.cpp")
set(GOOGLETEST_TEST_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc"
        )

# The App and Io source files must be sorted in two platform-specific lists:
#
# 1. ARM_BINARY_X86_COMPATIBLE_SRCS - These can be compiled on x86
# 2. ARM_BINARY_X86_INCOMPATIBLE_SRCS - These can't be compiled on x86
#
# Every App file can be compiled on x86. However, the same is not true for Io
# files. Some Io files rely on STM headers (e.g. stm32f3xx.h) and thus can't be
# compiled on x86. The rest of the Io files, however, can be compiled on x86.
#
# These relationships can be summarized as follows:
#
# ARM_BINARY_X86_COMPATIBLE_SRCS = App source files + x86-compatible Io files
# ARM_BINARY_X86_INCOMPATIBLE_SRCS = Io sources files - x86-compatible Io files
#
# On the other hand, the App and Io include directories are always
# x86-compatible because they are paths and not actual files. We just have to
# place App and Io include directories in ARM_BINARY_INCLUDE_DIRS.

# Prepare x86-compatible source files
set(ARM_BINARY_X86_COMPATIBLE_SRCS
        ${ARM_BINARY_APP_SRCS}
        ${X86_COMPATIBLE_IO_SRCS})

# Prepare x86-incompatible source files
list(REMOVE_ITEM ARM_BINARY_IO_SRCS "")
set(X86_INCOMPATIBLE_IO_SRCS "${ARM_BINARY_IO_SRCS}")
set(ARM_BINARY_X86_INCOMPATIBLE_SRCS ${X86_INCOMPATIBLE_IO_SRCS})

# Combine App and Io include directories
set(ARM_BINARY_INCLUDE_DIRS
        ${ARM_BINARY_APP_INCLUDE_DIRS}
        ${ARM_BINARY_IO_INCLUDE_DIRS})

create_arm_binary_or_tests_for_board(
    "DCM"
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
