#include "io/led.h"

void io_led_enable(const BinaryLed *led, const bool on)
{
    hw_gpio_writePin(led->gpio, on);
}
