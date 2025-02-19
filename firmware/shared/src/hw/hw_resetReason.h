#pragma once

#include <app_canUtils.h>

// Reset reasons for the STM32H733 MCU are described in the reference manual (RM0468).
// See "RCC reset status register (RCC_RSR)", section 8.7.38, page 430.

ResetReason hw_resetReason_get(void);
