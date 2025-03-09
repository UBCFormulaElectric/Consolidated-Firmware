#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpios.h"
#else
#include "app_utils.h"
#endif

/**
 * checks to see if right shutdown pin is high
 * @return true if high, false if low
 */
bool io_r_shdn_pin_is_high(void);

/**
 * checks to see if left shutdown pin is high
 * @return true if high, false if low
 */
bool io_l_shdn_pin_is_high(void);