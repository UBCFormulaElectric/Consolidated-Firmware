#pragma once

/**
 * @brief runs the power and current limit checking algorithm and updates all the current limit CAN messages
 */
void app_powerCurrentLimit_broadcast();

/**
 * @brief Gets the min discharge power Limit based on all of the temp
 * limiting condition
 * @return Minimum power current limit value
 */
float app_powerCurrentLimit_getDischargePowerLimit();

/**
 * @brief Gets the min charge power Limit based on all of the temp
 * limiting condition
 * @return Minimum power current limit value
 */
float app_powerCurrentLimit_getChargePowerLimit();

/**
 * @brief Gets the min discharge current Limit based on all of the current
 * limiting conditions
 * @return Minimum Discharge current limit value
 */
float app_powerCurrentLimit_getDischargeCurrentLimit();

/**
 * @brief Gets the min charge current limit based on all of the current
 * limiting conditions
 * @return Minimum Charge current limit value
 */
float app_powerCurrentLimit_getChargeCurrentLimit();

/**
 * @brief Gets the temperature based discharge current limit
 * @param max_cell_temp hottest measured temp of the series element
 * @return Temp based current limit
 */
float app_powerCurrentLimit_calcTempCurrentLimit(float max_cell_temp);

/**
 * @brief Gets the Low Voltage Clamp based discharge current limtit
 * @return Low Voltage Clamp based discharge current Limit value
 */
float app_powerCurrentLimit_calcLowVoltageCurrentLimit();

/**
 * @brief Gets the High Voltage Clamp based charge current limit
 * @return High Voltage Clamp based Charge current Limit value
 */
float app_powerCurrentLimit_calcHighVoltageClampCurrentLimit();

/**
 * @brief Gets the Low SOC based discharge current limit
 * @return Low SOC based discharge current Limit value
 */
float app_powerCurrentLimit_calcLowSOCCurrentLimit(); // TODO: implement this

/**
 * @brief Gets the High SOC based charge current limit
 * @return High SOC based charge current Limit value
 */
float app_powerCurrentLimit_calcHighSOCCurrentLimit(); // TODO: implement this
