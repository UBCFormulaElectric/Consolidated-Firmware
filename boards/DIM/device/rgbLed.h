#pragma once

#include "gpio.h"

typedef struct
{
    Gpio red_enable;
    Gpio green_enable;
    Gpio blue_enable;
} RgbLed;

void rgbLed_init(RgbLed *led, Gpio *red_enable, Gpio *green_enable, Gpio *blue_enable);
void rgbLed_enable(Led *led, bool red_on, bool green_on, bool blue_on);
void rgbLed_disable(Led *led);