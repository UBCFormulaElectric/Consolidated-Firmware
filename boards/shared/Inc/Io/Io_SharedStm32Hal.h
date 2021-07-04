#pragma once

// This header includes the correct stm32 HAL (Hardware abstraction layer)
// depending on what board this is being compiled for

#ifdef STM32F302xC
#include <stm32f3xx_hal.h>
#elif STM32F765xx
#include <stm32f7xx_hal.h>
#else
#error "No supported device define found."
#endif
