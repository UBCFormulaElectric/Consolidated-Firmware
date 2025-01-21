#pragma once
#include "hw_gpio.h"
#ifdef TARGET_EMBEDDED

typedef struct
{
    const Gpio *src;
} Fans;
void io_fan_set(const Fans *fans, bool on);

// extern const Fans acc_fan;
extern const Fans rad_fan;

#endif