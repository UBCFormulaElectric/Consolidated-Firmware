
#file(REMOVE_RECURSE ARM_BINARY_APP_SRCS "Src/App/lookup_tables/*.c")

#message(STATUS "arm_binary_app_srcs: ${ARM_BINARY_APP_SRCS}")

file(GLOB ARM_BINARY_APP_SRCS
        "Src/App/*.c"
        "Src/App/control/*.c"
        "Src/App/states/*.c"
        "Src/App/lookup_tables/motor_lut_interface.c"
        "Src/App/lookup_tables/shaft_torque_lut.c"
        "Src/App/lookup_tables/id_peak_lut.c"
        "Src/App/lookup_tables/iq_peak_lut.c")

set(ARM_BINARY_APP_INCLUDE_DIRS "Inc/App")
file(GLOB ARM_BINARY_IO_SRCS
        "Src/*.c"
        "Src/Io/*.c"
        "startup/*.s"
        )
#list(FILTER ARM_BINARY_IO_SRCS EXCLUDE REGEX ".*stm32f7xx_hal_msp.c")
#list(FILTER ARM_BINARY_IO_SRCS EXCLUDE REGEX ".*stm32f7xx_hal_hcd.c")

set(ARM_BINARY_IO_INCLUDE_DIRS
        "Inc"
        "Inc/Io"
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

set(ARM_BINARY_IO_SYSCALLS_SRC "Src/syscalls.c")
list(APPEND ARM_BINARY_THIRD_PARTY_SRCS ${ARM_BINARY_IO_SYSCALLS_SRC})
set(ARM_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32F765VGTx_FLASH.ld")

set(STM32CUBEMX_DRIVER_INCLUDE_DIRS
        "Drivers/STM32F7xx_HAL_Driver/Inc"
        "Drivers/STM32F7xx_HAL_Driver/Inc/Legacy"
        "Middlewares/Third_Party/FreeRTOS/Source/include"
        "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS"
        "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
#        "Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc"
#        "Middlewares/ST/STM32_USB_Device_Library/Core/Inc"
#        "Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc"
#        "Middlewares/ST/STM32_USB_Host_Library/Core/Inc"
        "Drivers/CMSIS/Device/ST/STM32F7xx/Include"
        "Drivers/CMSIS/Include"
        "Drivers/CMSIS/DSP/Include"
        )
list(APPEND ARM_BINARY_THIRD_PARTY_INCLUDE_DIRS ${STM32CUBEMX_DRIVER_INCLUDE_DIRS})
file(GLOB_RECURSE STM32CUBEMX_DRIVER_SRCS
        "Middlewares/Third_Party/*.c"
#        "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/*.c"
#        "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/*.c"
        "Drivers/STM32F7xx_HAL_Driver/*.c"
        )

#list(REMOVE_ITEM STM32CUBEMX_DRIVER_SRCS
#        "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_timebase_rtc_alarm_template.c"
#        )
list(FILTER STM32CUBEMX_DRIVER_SRCS EXCLUDE REGEX ".*template.*")
list(FILTER STM32CUBEMX_DRIVER_SRCS EXCLUDE REGEX ".*IAR.*")
message("${STM32CUBEMX_DRIVER_SRCS}")
list(APPEND ARM_BINARY_THIRD_PARTY_SRCS ${STM32CUBEMX_DRIVER_SRCS})


set(IOC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/INV.ioc")
set(STM32_DEVICE_TYPE STM32F765xx)

file(GLOB GOOGLETEST_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/*.cpp")
list(REMOVE_ITEM GOOGLETEST_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Test/Src/main.cpp")
set(GOOGLETEST_TEST_INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc"
        )

create_arm_binary_or_tests_for_board(
    "INV"
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
