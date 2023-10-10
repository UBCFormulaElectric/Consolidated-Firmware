file(GLOB_RECURSE ARM_BINARY_APP_SRCS "src/app/*.*")
set(ARM_BINARY_APP_INCLUDE_DIRS 
    "src/app"
)
file(GLOB ARM_BINARY_IO_SRCS
    "src/io/*.c"
    "src/dev/*.c"
    "src/cubemx/Src/*.c"
    "src/cubemx/startup/*.s"
)
set(ARM_BINARY_IO_INCLUDE_DIRS
    "src/io"
    "src/dev"
    "src/cubemx/Inc"
)

set(ARM_BINARY_X86_COMPATIBLE_SRCS
    ${ARM_BINARY_APP_SRCS}
    ${X86_COMPATIBLE_IO_SRCS})

list(REMOVE_ITEM ARM_BINARY_IO_SRCS "")
set(X86_INCOMPATIBLE_IO_SRCS "${ARM_BINARY_IO_SRCS}")
set(ARM_BINARY_X86_INCOMPATIBLE_SRCS ${X86_INCOMPATIBLE_IO_SRCS})

set(ARM_BINARY_INCLUDE_DIRS
    ${ARM_BINARY_APP_INCLUDE_DIRS}
    ${ARM_BINARY_IO_INCLUDE_DIRS})

set(ARM_BINARY_IO_SYSCALLS_SRC "src/cubemx/Src/syscalls.c")
list(APPEND ARM_BINARY_THIRD_PARTY_SRCS ${ARM_BINARY_IO_SYSCALLS_SRC})
set(ARM_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/STM32F412RGTx_FLASH.ld")

set(STM32CUBEMX_DRIVER_INCLUDE_DIRS
    "src/cubemx/Drivers/STM32F4xx_HAL_Driver/Inc"
    "src/cubemx/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy"
    "src/cubemx/Middlewares/Third_Party/FreeRTOS/Source/include"
    "src/cubemx/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS"
    "src/cubemx/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
    "src/cubemx/Drivers/CMSIS/Device/ST/STM32F4xx/Include"
    "src/cubemx/Drivers/CMSIS/Include"
)
list(APPEND ARM_BINARY_THIRD_PARTY_INCLUDE_DIRS ${STM32CUBEMX_DRIVER_INCLUDE_DIRS})
file(GLOB_RECURSE STM32CUBEMX_DRIVER_SRCS
    "src/cubemx/Middlewares/*.c"
    "src/cubemx/Drivers/*.c"
)
list(APPEND ARM_BINARY_THIRD_PARTY_SRCS ${STM32CUBEMX_DRIVER_SRCS})

set(IOC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DIM.ioc")
set(STM32_DEVICE_TYPE STM32F412Rx)

file(GLOB GOOGLETEST_TEST_SRCS "test/*.cpp")
list(REMOVE_ITEM GOOGLETEST_TEST_SRCS "test/main.cpp")
set(GOOGLETEST_TEST_INCLUDE_DIRS
    "test"
)

if("${PLATFORM}" STREQUAL "x86")
    create_fake_library(
        "DIM_fakes"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sevenSegDisplays.h"
    )
endif()


# INCLUDES and SOURCES from shared
# Define the shared code that you want to use in this project
set(SHARED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../shared")
file(GLOB_RECURSE SHARED_APP_SRCS "${SHARED_DIR}/src/app/*.c")
file(GLOB_RECURSE SHARED_IO_SRCS "${SHARED_DIR}/src/io/*.c")
file(GLOB_RECURSE SHARED_HW_SRCS "${SHARED_DIR}/src/hw/*.c")

set(SHARED_APP_INCLUDE_DIRS "${SHARED_DIR}/src/app")
set(SHARED_IO_INCLUDE_DIRS
    "${SHARED_DIR}/src/io"
    "${SHARED_DIR}/src/hw"
)


set(ALL_SHARED_SRCS
${SHARED_APP_SRCS}
${SHARED_IO_SRCS}
${SHARED_HW_SRCS}
)

set(ALL_SHARED_INCLUDE_DIRS
${SHARED_APP_INCLUDE_DIRS}
${SHARED_IO_INCLUDE_DIRS}
)

# list(REMOVE_ITEM ALL_SHARED_SRCS "${SHARED_DIR}/src/io/Io_SharedHardFaultHandler.c") 

message("SHARED_IO_SRCS: ${ALL_SHARED_SRCS}----------------------")



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
    "${ALL_SHARED_INCLUDE_DIRS}"
    "${ALL_SHARED_SRCS}"
    "${GOOGLETEST_TEST_SRCS}"
    "${GOOGLETEST_TEST_INCLUDE_DIRS}"
)
