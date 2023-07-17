#include <stm32f4xx.h>
#include "main.h"
#include "switch.h"

void switch_init(Switch *binary_switch, Gpio *input, bool active_state)
{
    binary_switch->input        = input;
    binary_switch->active_state = active_state;
}

bool switch_isEnabled(Switch *binary_switch)
{
    return gpio_read(binary_switch->input) == binary_switch->active_state;
}