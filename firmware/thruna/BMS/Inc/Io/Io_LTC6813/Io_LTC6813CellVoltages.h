#pragma once

#include "App_Accumulator.h"

/**
 * Read all cell voltages from the LTC6813
 * Note: Io_LTC6813CellVoltages_StartAdcConversion needs to be called to trigger
 * cell voltages conversions before valid cell voltages can be read back
 * @param cell_voltages Buffer to write out the read voltages to
 * @return True if cell voltages are read back successfully. Else, false
 */
bool Io_LTC6813CellVoltages_ReadVoltages(void);

/**
 * Start an ADC conversion on the LTC6813 for measured cell voltages
 * @return True if the ADC conversions have been sent out successfully. Else,
 * false
 */
bool Io_LTC6813CellVoltages_StartAdcConversion(void);

/**
 * @brief Read cell voltages from the previously stored values
 * @return Cell Voltage Value
 */
float Io_LTC6813CellVoltages_GetCellVoltage(uint8_t segement, uint8_t cell);
