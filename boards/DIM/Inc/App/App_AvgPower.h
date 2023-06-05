#pragma once

#include <stdbool.h>
#include <stdint.h>
/**
 *
 */
struct AvgPowerCalc;

/**
 *
 * @return
 */
struct AvgPowerCalc *App_AvgPowerCalc_Create(void);

/**
 *
 * @param avgPowerCalc
 */
void App_AvgPowerCalc_Destroy(struct AvgPowerCalc *avgPowerCalc);

/**
 *
 * @param avgPowerCalc
 * @return
 */
float App_AvgPowerCalc_Update(struct AvgPowerCalc *avgPowerCalc, float instant_power);

/**
 *
 * @param avgPowerCalc
 */
void App_AvgPowerCalc_Reset(struct AvgPowerCalc *avgPowerCalc);

/**
 *
 * @param avgPowerCalc
 * @param enabled
 */
void App_AvgPowerCalc_Enable(struct AvgPowerCalc *avgPowerCalc, bool enabled);
