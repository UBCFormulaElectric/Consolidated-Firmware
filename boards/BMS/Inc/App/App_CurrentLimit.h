#pragma once

#include "App_Soc.h"
#include "App_BmsWorld.h"
#include "App_Accumulator.h"
#include "App_SharedProcessing.h"

/**
 * Gets the the minimum discharge current limit
 * @param accumulator accumulator object for retrieving cell information
 * @param soc_stats the soc status of each series element
 * @return the discharge current limit
 */
float App_CurrentLimit_GetDischargeLimit(struct Accumulator *accumulator, struct SocStats *soc_stats);

/**
 * Gets the the minimum charge current limit
 * @param accumulator accumulator object for retrieving cell information
 * @param soc_stats the soc status of each series element
 * @return the charge current limit
 */
float App_CurrentLimit_GetChargeLimit(struct Accumulator *accumulator, struct SocStats *soc_stats);

/**
 * Gets the the temperature based discharge limit
 * @param max_cell_temp temp of hottest series element
 * @return the discharge current limit
 */
float App_CurrentLimit_CalculateTempBasedDischargeLimit(float max_cell_temp);

/**
 * Gets the the SOC based discharge limit
 * @param soc_stats the soc status of each series element
 * @return the discharge current limit
 */
float App_CurrentLimit_CalculateLowCellVoltageClampDischargeLimit(struct SocStats *soc_stats);

/**
 * Gets the SOC based charge current limit
 * @param Vmax the max cell voltage of the accumulator
 * @param soc_stats the soc status of each series element
 * @return the charge current limit
 */
float App_CurrentLimit_CalculateHighCellVoltageClampChargeLimit(float Vmax, struct SocStats *soc_stats);
