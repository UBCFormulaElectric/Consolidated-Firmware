#pragma once

#include <stdbool.h>
#include "gpio.h"

typedef struct
{
    Gpio input;
    bool active_state;
} Switch;

void switch_init(Switch *binary_switch, Gpio *input, bool active_state);
bool switch_isEnabled(Switch *binary_switch);
