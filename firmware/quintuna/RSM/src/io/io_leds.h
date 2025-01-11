#pragma once
#include "io_led.h"
#include "hw_gpios.h"

typedef struct
{
    Gpio *src;
} BinaryLed;

extern const BinaryLed brake_light;
extern const BinaryLed led;