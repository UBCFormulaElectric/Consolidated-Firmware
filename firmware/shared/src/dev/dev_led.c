#include "dev_led.h"

void dev_led_enable(const BinaryLed *led, bool on)
{
    hw_gpio_writePin(&led->gpio, on);
}
