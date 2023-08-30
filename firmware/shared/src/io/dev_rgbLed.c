#include "dev_rgbLed.h"

void dev_rgbLed_enable(const RgbLed *rgb_led, bool red_on, bool green_on, bool blue_on)
{
    hw_gpio_writePin(&rgb_led->red_gpio, red_on);
    hw_gpio_writePin(&rgb_led->green_gpio, green_on);
    hw_gpio_writePin(&rgb_led->blue_gpio, blue_on);
}

void dev_rgbLed_disable(const RgbLed *rgb_led)
{
    dev_rgbLed_enable(rgb_led, false, false, false);
}