################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Io_Can.c \
../Src/freertos.c \
../Src/main.c \
../Src/stm32f3xx_hal_msp.c \
../Src/stm32f3xx_hal_timebase_tim.c \
../Src/stm32f3xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f3xx.c 

OBJS += \
./Src/Io_Can.o \
./Src/freertos.o \
./Src/main.o \
./Src/stm32f3xx_hal_msp.o \
./Src/stm32f3xx_hal_timebase_tim.o \
./Src/stm32f3xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f3xx.o 

C_DEPS += \
./Src/Io_Can.d \
./Src/freertos.d \
./Src/main.d \
./Src/stm32f3xx_hal_msp.d \
./Src/stm32f3xx_hal_timebase_tim.d \
./Src/stm32f3xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f3xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Io_Can.o: ../Src/Io_Can.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F302x8 -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core_A/Include/ -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/DSP/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc/auto_generated" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I"/home/gareth/programming/Consolidated-Firmware/boards/BMS/Inc" -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core/Include/ -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Io_Can.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/freertos.o: ../Src/freertos.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F302x8 -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core_A/Include/ -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/DSP/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc/auto_generated" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I"/home/gareth/programming/Consolidated-Firmware/boards/BMS/Inc" -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core/Include/ -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/freertos.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F302x8 -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core_A/Include/ -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/DSP/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc/auto_generated" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I"/home/gareth/programming/Consolidated-Firmware/boards/BMS/Inc" -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core/Include/ -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f3xx_hal_msp.o: ../Src/stm32f3xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F302x8 -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core_A/Include/ -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/DSP/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc/auto_generated" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I"/home/gareth/programming/Consolidated-Firmware/boards/BMS/Inc" -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core/Include/ -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f3xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f3xx_hal_timebase_tim.o: ../Src/stm32f3xx_hal_timebase_tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F302x8 -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core_A/Include/ -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/DSP/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc/auto_generated" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I"/home/gareth/programming/Consolidated-Firmware/boards/BMS/Inc" -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core/Include/ -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f3xx_hal_timebase_tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f3xx_it.o: ../Src/stm32f3xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F302x8 -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core_A/Include/ -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/DSP/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc/auto_generated" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I"/home/gareth/programming/Consolidated-Firmware/boards/BMS/Inc" -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core/Include/ -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f3xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F302x8 -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core_A/Include/ -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/DSP/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc/auto_generated" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I"/home/gareth/programming/Consolidated-Firmware/boards/BMS/Inc" -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core/Include/ -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32f3xx.o: ../Src/system_stm32f3xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F302x8 -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core_A/Include/ -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc" -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/DSP/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/gareth/programming/Consolidated-Firmware/boards/shared/Inc/auto_generated" -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I"/home/gareth/programming/Consolidated-Firmware/boards/BMS/Inc" -I../Middlewares/Third_Party/ARM_CMSIS//CMSIS/Core/Include/ -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f3xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

