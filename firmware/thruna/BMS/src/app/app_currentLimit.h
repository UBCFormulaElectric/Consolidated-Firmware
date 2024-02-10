#pragma once

/**
 * @brief runs the current limit checking algorithm and updates all the current limit CAN messages
 */
void app_currentLimit_broadcast(void);

/**
 * @brief Gets the min discharge current Limit based on all of the current 
 * limiting conditions
 * @return Minimum Discharge current limit value
 */
float app_currentLimit_getDischargeLimit(void);

/**
 * @brief Gets the min charge current limit based on all of the current
 * limiting conditions
 * @return Minimum Charge current limit value
*/
float app_currentLimit_getChargeLimit(void);

/**
 * @brief Gets the temperature based discharge current limit
 * @param measured_max_cell_temp hottest measured temp of the series element 
 * @return Temp based current limit
 */
float app_currentLimit_calcTempCurrentLimit(float measured_max_cell_temp);

/**
 * @brief Gets the Low Voltage Clamp based discharge current limtit 
 * @return Low Voltage Clamp based discharge current Limit value
 */
float app_currentLimit_calcLowVoltClampCurrentLimit(void);

/**
 * @brief Gets the High Voltage Clamp based charge current limit
 * @return High Voltage Clamp based Charge current Limit value
 */
float app_currentLimit_calcHighVoltClampCurrentLimit(void);

/**
 * @brief Gets the Low SOC based discharge current limit
 * @return Low SOC based discharge current Limit value
 */
float app_currentLimit_calcLowSocCurrentLimit(void);

/**
 * @brief Gets the High SOC based charge current limit
 * @return High SOC based charge current Limit value
 */
float app_currentLimit_calcHighSocCurrentLimit(void);
