#include "dev_binarySwitch.h"

bool dev_binarySwitch_isOn(const BinarySwitch* binary_switch)
{
    return hw_gpio_readPin(binary_switch->pin) == binary_switch->on_state;
}
