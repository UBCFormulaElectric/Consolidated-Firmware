#pragma once

#include "App_Accumulator.h"

/**
 * Read all cell voltages from the LTC6813
 * Note: Io_LTC6813CellVoltages_StartAdcConversion needs to be called to trigger
 * cell voltages conversions before valid cell voltages can be read back
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
 * Get a one-hot coded number indicating cells to discharge
 * @param segment The segment containing cells to discharge
 * @return A one-hot coded number indicating cells to discharge. Within the
 * hot-coded number 1 indicates discharge, while 0 indicates no discharge
 */
uint16_t Io_LTC6813CellVoltages_GetCellsToDischarge(AccumulatorSegments_E segment);

/**
 * Get the location of the minimum cell voltage
 * @param segment The segment of the minimum cell voltage
 * @param cell The cell location of the minimum cell voltage
 * @return The minimum cell voltage in (V)
 */
float Io_LTC6813CellVoltages_GetMinCellVoltage(uint8_t *segment, uint8_t *cell);

/**
 * Get the maximum cell voltage in (V)
 * @param segment The segment of the maximum cell voltage
 * @param cell The cell location of the maximum cell voltage
 * @return The maximum cell voltage in (V)
 */
float Io_LTC6813CellVoltages_GetMaxCellVoltage(uint8_t *segment, uint8_t *cell);

/**
 * Get the segment voltage in (V)
 * @param segment The segment to get the segment voltage from
 * @return The segment voltage in (V)
 */
float Io_LTC6813CellVoltages_GetSegmentVoltage(AccumulatorSegments_E segment);

/**
 * Get the pack voltage in (V)
 * @return The pack voltage in (V)
 */
float Io_LTC6813CellVoltages_GetPackVoltage(void);

/**
 * Get average cell voltage in (V)
 * @return The average cell voltage in (V)
 */
float Io_LTC6813CellVoltages_GetAverageCellVoltage(void);
