#include "hw_rgbLed.h"

void hw_rgbLed_enable(const RgbLed *rgb_led, RgbLedColour colour)
{
    bool red_on   = false;
    bool blue_on  = false;
    bool green_on = false;
    if (colour == RGB_RED || colour == RGB_YELLOW || colour == RGB_MAGENTA || colour == RGB_WHITE)
        red_on = true;
    if (colour == RGB_BLUE || colour == RGB_CYAN || colour == RGB_MAGENTA || colour == RGB_WHITE)
        blue_on = true;
    if (colour == RGB_GREEN || colour == RGB_YELLOW || colour == RGB_CYAN || colour == RGB_WHITE)
        green_on = true;
    hw_gpio_writePin(&rgb_led->red_gpio, red_on);
    hw_gpio_writePin(&rgb_led->green_gpio, green_on);
    hw_gpio_writePin(&rgb_led->blue_gpio, blue_on);
}

void hw_rgbLed_disable(const RgbLed *rgb_led)
{
    hw_rgbLed_enable(rgb_led, RGB_OFF);
}
