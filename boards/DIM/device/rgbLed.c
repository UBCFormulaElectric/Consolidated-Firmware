#include "rgb+led.h"
#include <stm32f4xx.h>

void rgb_led_init(RgbLed *led, Gpio *red_enable, Gpio *green_enable, Gpio *blue_enable)
{
    led->red_enable   = red_enable;
    led->green_enable = green_enable;
    led->blue_enable  = blue_enable;
}

void rgb_led_enable(RgbLed *led, bool red_on, bool green_on, bool blue_on)
{
    gpio_write(led->red_enable, red_on);
    gpio_write(led->red_enable, green_on);
    gpio_write(led->red_enable, blue_on);
}

void rgb_led_disable(RgbLed *led);
{
    rgb_led_enable(led, false, false, false);
}
