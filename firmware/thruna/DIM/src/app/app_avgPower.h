#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Initialize average power counter
 */
void app_avgPower_init(void);

/**
 * Calculate the running averaged power value and update the counter
 * @return the current and running value for the averaged power value
 */
float app_avgPower_update(float instant_power);

/**
 * Reset all values, set running average and counter to 0
 */
void app_avgPower_reset(void);

/**
 * Enable the average power calculation struct
 * @param enabled Whether or not to enable the avg power calculation.
 */
void app_avgPower_enable(bool enabled);
