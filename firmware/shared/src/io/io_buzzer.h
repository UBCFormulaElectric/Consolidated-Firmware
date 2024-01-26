#pragma once

#include <stdbool.h>
#include "App_SharedMacros.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio gpio;
} Buzzer;
#else
EMPTY_STRUCT(Buzzer);
#endif

/**
 * Turn a buzzer on or off.
 * @param on Whether or not to turn buzzer on.
 */
void io_buzzer_enable(const Buzzer *buzzer, bool on);
