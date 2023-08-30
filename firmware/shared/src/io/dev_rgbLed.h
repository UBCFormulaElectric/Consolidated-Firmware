#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
#endif

typedef struct
{
#ifdef TARGET_EMBEDDED
    const Gpio red_gpio;
    const Gpio green_gpio;
    const Gpio blue_gpio;
#endif
} RgbLed;

/**
 * Enable an RGB LED.
 * @param rgb_led RGB LED instance.
 * @param red_on Whether or not to turn on the red LED.
 * @param green_on Whether or not to turn on the green LED.
 * @param blue_on Whether or not to turn on the blue LED.
 */
void dev_rgbLed_enable(const RgbLed *rgb_led, bool red_on, bool green_on, bool blue_on);

/**
 * Disable an RGB LED (turn all LEDs off).
 * @param rgb_led RGB LED instance.
 */
void dev_rgbLed_disable(const RgbLed *rgb_led);