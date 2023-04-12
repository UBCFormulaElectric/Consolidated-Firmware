#pragma once

#include "App_Accumulator.h"

/**
 * Read all cell voltages from the LTC6813
 * Note: Io_LTC6813CellVoltages_StartAdcConversion needs to be called to trigger
 * cell voltages conversions before valid cell voltages can be read back
 * @param cell_voltages Buffer to write out the read voltages to
 * @param num_readings_per_segment Number of voltages to read back per segment.
 * 16 for voltage monitoring, 17 for open wire check.
 * @return True if cell voltages are read back successfully. Else, false
 */
bool Io_LTC6813CellVoltages_ReadVoltages(float *cell_voltages, uint8_t num_readings_per_segment);

/**
 * Start an ADC conversion on the LTC6813 for measured cell voltages
 * @return True if the ADC conversions have been sent out successfully. Else,
 * false
 */
bool Io_LTC6813CellVoltages_StartAdcConversion(void);

/**
 * Start an open wire check on the LTC6813, used to detect if any connections from the ADCs to the cells have gone open
 * circuit. The command performs ADC conversions identically to the ADCV command, except with either a pull up or pull
 * down current.
 * @param pull_up If true, run command with pull-up current. Otherwise, run with pull-down.
 * @return True if the ADOW commands have been sent out successfully. Else, false
 */
bool Io_LTC6813CellVoltages_StartOpenWireCheck(bool pull_up);

/**
 * Get a one-hot coded number indicating cells to discharge
 * @param segment The segment containing cells to discharge
 * @return A one-hot coded number indicating cells to discharge. Within the
 * hot-coded number 1 indicates discharge, while 0 indicates no discharge
 */
uint16_t Io_LTC6813CellVoltages_GetCellsToDischarge(AccumulatorSegment segment);
