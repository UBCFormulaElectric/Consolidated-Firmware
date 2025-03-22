#pragma once

#include <stdbool.h>
<<<<<<< HEAD
=======
// Question: what is redundant here now that the config struct gone?

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

#else
>>>>>>> 5e8451f7b (Squashed commit of the following:)
#include "app_utils.h"

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
