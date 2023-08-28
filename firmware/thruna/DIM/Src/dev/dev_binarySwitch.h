#pragma once

#include <stdbool.h>
#include "hw_gpio.h"

typedef struct {
    const Gpio* const pin;
    const bool on_state;
} BinarySwitch;

/**
 * Check if the switch is turned on (i.e. matches its `on_state`).
 * @param binary_switch Binary switch instance.
 * @return True if the switch is turned on, else false.
 */
bool dev_binarySwitch_isOn(const BinarySwitch* binary_switch);
