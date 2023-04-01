#pragma once

#include "App_Soc.h"
#include "App_BmsWorld.h"
#include "App_Accumulator.h"
#include "App_SharedProcessing.h"

/**
 * Gets the the minimum discharge current limit
 * @param accumulator the accumulator object
 * @param soc_stats the soc status of each series element
 * @return the discharge current limit
 */
float App_CurrentLimit_GetDischargeLimit(struct Accumulator *accumulator, struct SocStats *soc_stats);

/**
 * Gets the the minimum charge current limit
 * @param none
 * @return the charge current limit
 */
float App_CurrentLimit_GetChargeLimit(void);

/**
 * Gets the the appropriate discharge current limit
 * @param max_cell_temp the max temp of all series elements
 * @return the discharge current limit
 */
float App_CurrentLimit_CalculateTempBasedDischargeLimit(float max_cell_temp);

/**
 * Gets the the appropriate discharge current limit
 * @param soc_stats the soc status of each series element
 * @return the charge current limit
 */
float App_CurrentLimit_CalculateLowCellVoltageClampDischargeLimit(struct SocStats *soc_stats);
