#pragma once

// This header includes the correct stm32 drivers header depending on what
// board this is being compiled for

#ifdef STM32F302xC
#include <stm32f3xx.h>
#elif STM32F765xx
#include <stm32f7xx.h>
#else
#error "No supported device define found."
#endif
