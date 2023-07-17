#pragma once

#include "gpio.h"

typedef struct
{
    Gpio output;
} Led;

void led_init(Led *led, Gpio *output);
void led_enable(Led *led, bool on);
