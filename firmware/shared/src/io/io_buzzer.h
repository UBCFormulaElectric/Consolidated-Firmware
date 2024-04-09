#pragma once

#include <stdbool.h>
#include "app_utils.h"

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
void io_buzzer_enable(bool on);

/**
 * Intialize Buzzer as off
 * @return buzzer
 */
void io_buzzer_init(const Buzzer *buzzer_in);
