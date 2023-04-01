#pragma once

#include "App_Soc.h"
#include "App_BmsWorld.h"
#include "App_Accumulator.h"
#include "App_SharedProcessing.h"

/**
 * Gets the the minimum discharge current limit
 * @param none
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
 * @param max_cell_temp
 * @return the discharge current limit
 */
float App_CurrentLimit_CalculateTempBasedDischargeLimit(float max_cell_temp);

/**
 * Gets the the appropriate charge discurrent limit
 * @param min_cell_soc, max_cell_soc
 * @return the charge current limit
 */
float App_CurrentLimit_CalculateSOCBasedDischargeLimit(float min_cell_soc);

/**
 * Gets the the appropriate discharge current limit
 * @param min_cell_voltage
 * @return the charge current limit
 */
float App_CurrentLimit_CalculateLowCellVoltageClampDischargeLimit(struct SocStats *soc_stats)
