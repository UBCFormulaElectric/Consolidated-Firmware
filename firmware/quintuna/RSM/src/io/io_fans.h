#pragma once

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio *src;
} Fans;
#else
#include "app_utils.h"
EMPTY_STRUCT(Fans);
#endif

void io_fan_set(const Fans *fans, bool on);

// extern const Fans acc_fan;
extern const Fans rad_fan;

