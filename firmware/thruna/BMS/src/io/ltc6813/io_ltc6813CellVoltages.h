#pragma once

/**
 * Read all cell voltages from the LTC6813
 * Note: Io_LTC6813CellVoltages_StartAdcConversion needs to be called to trigger
 * cell voltages conversions before valid cell voltages can be read back
 * @return True if cell voltages are read back successfully. Else, false
 */
bool io_ltc6813CellVoltages_readVoltages(void);

/**
 * Start an ADC conversion on the LTC6813 for measured cell voltages
 * @return True if the ADC conversions have been sent out successfully. Else,
 * false
 */
bool io_ltc6813CellVoltages_startAdcConversion(void);

/**
 * @brief Read cell voltages from the previously stored values
 * @return Cell Voltage Value
 */
float io_ltc6813CellVoltages_getCellVoltage(uint8_t segement, uint8_t cell);