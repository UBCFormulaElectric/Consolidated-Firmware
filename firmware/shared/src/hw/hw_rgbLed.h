#pragma once

#include <stdbool.h>
#include "hw_gpio.h"

typedef struct
{
    const Gpio red_gpio;
    const Gpio green_gpio;
    const Gpio blue_gpio;
} RgbLed;

typedef enum
{
    RGB_RED,
    RGB_GREEN,
    RGB_BLUE,
    RGB_YELLOW,
    RGB_CYAN,
    RGB_MAGENTA,
    RGB_WHITE,
    RGB_OFF
} RgbLedColour;

/**
 * Enable an RGB LED.
 * @param rgb_led RGB LED instance.
 * @param colour Colour to enable.
 */
void hw_rgbLed_enable(const RgbLed *rgb_led, RgbLedColour colour);

/**
 * Disable an RGB LED (turn all LEDs off).
 * @param rgb_led RGB LED instance.
 */
void hw_rgbLed_disable(const RgbLed *rgb_led);
