#pragma once

#include "hw_gpio.h"
#include <stdbool.h>

typedef struct
{
    const Gpio* const red_enable;
    const Gpio* const green_enable;
    const Gpio* const blue_enable;
} RgbLed;

/**
 * Enable an RGB LED.
 * @param red_on Whether or not to turn on the red LED.
 * @param green_on Whether or not to turn on the green LED.
 * @param blue_on Whether or not to turn on the blue LED.
 */
void dev_rgbLed_enable(const RgbLed *rgb_led, bool red_on, bool green_on, bool blue_on);

/**
 * Disable an RGB LED (turn all LEDs off).
 */
void dev_rgbLed_disable(const RgbLed *rgb_led);