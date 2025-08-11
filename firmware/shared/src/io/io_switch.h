#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio *gpio;
    bool        closed_state;
    bool        debounced_state;    // last stable state
    bool        last_raw_state;     // last reading
    uint32_t    last_debounce_time; // raw state last changed
} Switch;
#else
#include "app_utils.h"
EMPTY_STRUCT(Switch);
#endif

/**
 * Check if the switch is turned on (i.e. matches its `on_state`).
 * @param binary_switch Binary switch instance.
 * @return True if the switch is turned on, else false.
 */
bool io_switch_isClosed(Switch *sw);
