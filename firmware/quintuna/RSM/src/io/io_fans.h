#pragma once
#include "hw_gpios.h"

typedef struct
{
    const Gpio *src;
} Fans;

void io_fan_set(const Fans *fans, bool on);

// extern const Fans acc_fan;
extern const Fans rad_fan;