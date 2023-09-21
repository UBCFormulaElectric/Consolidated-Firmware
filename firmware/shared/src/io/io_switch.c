#include "io_switch.h"

bool io_switch_isClosed(const Switch *binary_switch)
{
    return hw_gpio_readPin(&binary_switch->gpio) == binary_switch->closed_state;
}
