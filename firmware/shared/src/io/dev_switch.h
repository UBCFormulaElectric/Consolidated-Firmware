#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
#endif

typedef struct
{
#ifdef TARGET_EMBEDDED
    const Gpio gpio;
    const bool closed_state; // GPIO state that indicates closed.
#endif
} Switch;

/**
 * Check if the switch is turned on (i.e. matches its `on_state`).
 * @param binary_switch Binary switch instance.
 * @return True if the switch is turned on, else false.
 */
bool dev_switch_isClosed(const Switch *binary_switch);