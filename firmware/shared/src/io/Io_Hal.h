/**
 * @brief Conditionally includes different versions of STM32 HAL drivers, allowing for compiling for different
 * families of MCUs. Include this file to automatically include the target board's version of STM32 HAL (stm32fXxx.h).
 * This also defines board-specific macros for peripheral drivers. For example, to use SPI in shared code, #include
 * IO_HAL_SPI which is defined to stm32f3xx_hal_spi.h on F3 and stm32f4xx_hal_spi.h on F4.
 */

#pragma once

#if defined(STM32F302xC)

#include <stm32f3xx.h>
#define IO_HAL_SPI <stm32f3xx_hal_spi.h>
#define IO_HAL_ADC <stm32f3xx_hal_adc.h>

#elif defined(STM32F412Rx)

#define IO_HAL_SPI <stm32f4xx_hal_spi.h>
#define IO_HAL_ADC <stm32f4xx_hal_adc.h>
#include <stm32f4xx.h>

#endif
