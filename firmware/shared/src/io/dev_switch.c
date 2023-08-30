#include "dev_switch.h"

bool dev_switch_isClosed(const Switch *binary_switch)
{
    return hw_gpio_readPin(&binary_switch->gpio) == binary_switch->closed_state;
}
