#pragma once

#include <stdbool.h>
// Question: what is redundant here now that the config struct gone?

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

#else
#include "app_utils.h"
EMPTY_STRUCT(TsimLightConfig)
#endif

/**
 * Turn on the buzzer
 */
void io_enable_buzzer(void);

/**
 * Turn off the buzzer
 */
void io_disable_buzzer(void);