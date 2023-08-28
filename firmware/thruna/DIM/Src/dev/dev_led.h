#pragma once

#include <stdbool.h>
#include "hw_gpio.h"

typedef struct {
    const Gpio* const pin;
} Led;

/**
 * Turn an LED on or off.
 * @param on Turn on if true, turn off if false.
 */
void dev_led_enable(const Led* led, bool on);
