#pragma once

#include <stdbool.h>
#include "utils.h"

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
