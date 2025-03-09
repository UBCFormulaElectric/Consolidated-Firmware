#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpios.h"
#else
#include "app_utils.h"
#endif

/**
 * Turn on the red light on the TSIM
 */
void io_tsim_set_red_high(void);

/**
 * Turn off the red light on the TSIM
 */
void io_tsim_set_red_low(void);

/**
 * Turn on the green light on the TSIM
 */
void io_tsim_set_green_high(void);

/**
 * Turn off the green light on the TSIM
 */
void io_tsim_set_green_low(void);
