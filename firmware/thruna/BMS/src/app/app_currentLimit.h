#pragma once


float app_currentLimit_getDischargeLimit(void);

/**
 * @brief Gets the temperature based discharge current limit
 * @param measured_max_cell_temp hottest measured temp of the series element 
 * @return the discharge current limit
 */
float app_currentLimit_calcTempCurrentLimit(float measured_max_cell_temp);


float app_currentLimit_calcLowVoltClampCurrentLimit(void);


float app_currentLimit_calcHighVoltClampCurrentLimit(void);


float app_currentLimit_calcLowSocCurrentLimit(void);
