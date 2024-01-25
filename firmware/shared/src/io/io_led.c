#include "io_led.h"

void io_led_enable(const BinaryLed *led, bool on)
{
    hw_gpio_writePin(&led->gpio, on);
}
