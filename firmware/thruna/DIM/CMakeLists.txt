file(GLOB_RECURSE ARM_BINARY_APP_SRCS "src/app/*.*")
set(ARM_BINARY_APP_INCLUDE_DIRS
    "src/app"
)
file(GLOB ARM_BINARY_IO_SRCS
    "src/io/*.c"
    "src/dev/*.c"
    "src/cubemx/Src/*.c"
    "src/cubemx/Startup/*.s"
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
set(ARM_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/STM32F412RGTX_FLASH.ld")

set(DRIVERS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Drivers")
set(FREERTOS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/Middlewares/Third_Party/FreeRTOS/Source")
set(STM32CUBEMX_DRIVER_INCLUDE_DIRS
    "${DRIVERS_SRC_DIR}/STM32F4xx_HAL_Driver/Inc"
    "${DRIVERS_SRC_DIR}/CMSIS/Include"
    "${DRIVERS_SRC_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy"
    "${DRIVERS_SRC_DIR}/CMSIS/Device/ST/STM32F4xx/Include"
    "${FREERTOS_SRC_DIR}/include"
    "${FREERTOS_SRC_DIR}/CMSIS_RTOS_V2"
    "${FREERTOS_SRC_DIR}/portable/GCC/ARM_CM4F"
)
list(APPEND ARM_BINARY_THIRD_PARTY_INCLUDE_DIRS ${STM32CUBEMX_DRIVER_INCLUDE_DIRS})
file(GLOB_RECURSE STM32CUBEMX_DRIVER_SRCS
    "${DRIVERS_SRC_DIR}/*.c"
    "${FREERTOS_SRC_DIR}/*.c"
)
list(APPEND ARM_BINARY_THIRD_PARTY_SRCS ${STM32CUBEMX_DRIVER_SRCS})

# We use heap_useNewlib.c for a thread-safe heap implementation, so we can use malloc with FreeRTOS.
# FreeRTOS also provides heap implementations, and when using the CMSIS_V2 interface we are required to pick one.
# FreeRTOS's heap will cause linker errors with heap_useNewlib.c, so remove all possible FreeRTOS heap implementations from the build.
# Source: https://nadler.com/embedded/newlibAndFreeRTOS.html
list(REMOVE_ITEM ARM_BINARY_THIRD_PARTY_SRCS "${FREERTOS_SRC_DIR}/portable/MemMang/heap_1.c")
list(REMOVE_ITEM ARM_BINARY_THIRD_PARTY_SRCS "${FREERTOS_SRC_DIR}/portable/MemMang/heap_2.c")
list(REMOVE_ITEM ARM_BINARY_THIRD_PARTY_SRCS "${FREERTOS_SRC_DIR}/portable/MemMang/heap_3.c")
list(REMOVE_ITEM ARM_BINARY_THIRD_PARTY_SRCS "${FREERTOS_SRC_DIR}/portable/MemMang/heap_4.c")
list(REMOVE_ITEM ARM_BINARY_THIRD_PARTY_SRCS "${FREERTOS_SRC_DIR}/portable/MemMang/heap_5.c")

set(IOC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/cubemx/DIM.ioc")
set(STM32_DEVICE_TYPE STM32F412Rx)

file(GLOB GOOGLETEST_TEST_SRCS "test/*.cpp")
list(REMOVE_ITEM GOOGLETEST_TEST_SRCS "test/main.cpp")
set(GOOGLETEST_TEST_INCLUDE_DIRS "test")

if("${PLATFORM}" STREQUAL "x86")
    create_fake_library(
        "DIM_fakes"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io/io_sevenSegDisplays.h"
    )
endif()


# INCLUDES and SOURCES from shared
# Define the shared code that you want to use in this project
set(SELECTED_SHARED_SRCS
    ${ALL_SHARED_APP_SRCS}
    ${ALL_SHARED_IO_SRCS}
    ${ALL_SHARED_HW_SRCS}
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
    "${SELECTED_SHARED_SRCS}"
    "${GOOGLETEST_TEST_SRCS}"
    "${GOOGLETEST_TEST_INCLUDE_DIRS}"
)