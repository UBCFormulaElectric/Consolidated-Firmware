#pragma once

#include "App_Accumulator.h"

/**
 * Read all cell voltages from the LTC6813
 * Note: Io_LTC6813CellVoltages_StartAdcConversion needs to be called to trigger
 * cell voltages conversions before valid cell voltages can be read back
 * @param cell_voltages Buffer to write out the read voltages to
 * @return True if cell voltages are read back successfully. Else, false
 */
bool Io_LTC6813CellVoltages_ReadVoltages(
    float cell_voltages[ACCUMULATOR_NUM_SEGMENTS]
                       [ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT]);

/**
 * Start an ADC conversion on the LTC6813 for measured cell voltages
 * @return True if the ADC conversions have been sent out successfully. Else,
 * false
 */
bool Io_LTC6813CellVoltages_StartAdcConversion(void);

/**
 * Get a one-hot coded number indicating cells to discharge
 * @param segment The segment containing cells to discharge
 * @return A one-hot coded number indicating cells to discharge. Within the
 * hot-coded number 1 indicates discharge, while 0 indicates no discharge
 */
uint16_t Io_LTC6813CellVoltages_GetCellsToDischarge(AccumulatorSegment segment);
