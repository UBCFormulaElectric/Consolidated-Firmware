#include "dev_led.h"

void dev_led_enable(const Led* led, bool on)
{
    hw_gpio_writePin(led->pin, on);
}
