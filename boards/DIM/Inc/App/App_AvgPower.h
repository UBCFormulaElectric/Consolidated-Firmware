#pragma once

#include <stdbool.h>
#include <stdint.h>
/**
 * A struct used to keep track of and calculate the averaged power given at
 * since enabled.
 */
struct AvgPowerCalc;

/**
 * Allocate the initial average power counter
 * @return avgPowerCalc
 */
struct AvgPowerCalc *App_AvgPowerCalc_Create(void);

/**
 * Deallocate the memory used by the given average power struct
 * @param avgPowerCalc
 */
void App_AvgPowerCalc_Destroy(struct AvgPowerCalc *avgPowerCalc);

/**
 * Calculate the running averaged power value and update the counter
 * @param avgPowerCalc
 * @return the current and running value for the averaged power value
 */
float App_AvgPowerCalc_Update(struct AvgPowerCalc *avgPowerCalc, float instant_power);

/**
 * Reset all values, set running average and counter to 0
 * @param avgPowerCalc
 */
void App_AvgPowerCalc_Reset(struct AvgPowerCalc *avgPowerCalc);

/**
 * Enable the average power calculation struct
 * @param avgPowerCalc
 * @param enabled
 */
void App_AvgPowerCalc_Enable(struct AvgPowerCalc *avgPowerCalc, bool enabled);
