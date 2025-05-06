#pragma once

// Reset reasons for the STM32H733 MCU are described in the reference manual (RM0468).
// See "RCC reset status register (RCC_RSR)", section 8.7.38, page 430.

// Reset reasons for the STM32F412 MCU are described in the reference manual (RM0402).
// See "RCC clock control & status register (RCC_CSR)", section 6.3.21, page 156.

// Must be kept up to date with `CanResetReason` in `shared_enum.json`!!!
typedef enum
{
    // No specific reset cause identified. Take a look at RCC_RSR/RCC_CSR for
    // more information (there are a few possible reset reasons I've omitted for
    // clarity since I don't think we'd ever trigger them, but I could be wrong!).
    RESET_REASON_UNKNOWN = 0,
    // Reset caused by watchdog timeout.
    RESET_REASON_WATCHDOG = 1,
    // Reset triggered by software or debugger.
    RESET_REASON_SOFTWARE = 2,
    // Power-on/power-down reset.
    RESET_REASON_POWER_ON = 3,
    // External reset via the NRST pin.
    RESET_REASON_NRST_PIN = 4,
    // Brownout reset (voltage drop below threshold).
    RESET_REASON_BROWN_OUT = 5,
} ResetReason;

/*
 * Returns the reason the microcontroller reset.
 */
ResetReason hw_resetReason_get(void);
