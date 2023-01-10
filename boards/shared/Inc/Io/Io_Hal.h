#pragma once


#if defined(STM32F302xC)
#define _TARGET_STM32F3
#elif defined(STM32F412Rx)
#define _TARGET_STM32F4
#endif


#if defined(_TARGET_STM32F3)

#include <stm32f3xx.h>
#define IO_HAL_FREERTOS_VERSION TRC_FREERTOS_VERSION_9_0_0
#define IO_HAL_SPI <stm32f3xx_hal_spi.h>

#elif defined(_TARGET_STM32F4)

#include <stm32f4xx.h>
#define IO_HAL_FREERTOS_VERSION TRC_FREERTOS_VERSION_10_3_1
#define IO_HAL_SPI <stm32f4xx_hal_spi.h>

#endif

