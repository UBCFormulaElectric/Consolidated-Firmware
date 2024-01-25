#pragma once

#include <stdbool.h>
#include "App_SharedMacros.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio gpio;
    const bool closed_state; // GPIO state that indicates closed.
} Switch;
#else
EMPTY_STRUCT(Switch);
#endif

/**
 * Check if the switch is turned on (i.e. matches its `on_state`).
 * @param binary_switch Binary switch instance.
 * @return True if the switch is turned on, else false.
 */
bool io_switch_isClosed(const Switch *binary_switch);