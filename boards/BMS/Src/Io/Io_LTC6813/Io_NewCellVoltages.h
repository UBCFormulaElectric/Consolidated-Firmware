#pragma once

#include "App_Accumulator.h"

/**
 * Read all cell voltages from the LTC6813
 * Note: Io_NewCellVoltages_StartAdcConversion needs to be called to trigger
 * cell voltages conversions before valid cell voltages can be read back
 * @return True if cell voltages are read back successfully. Else, false
 */
bool Io_NewCellVoltages_ReadVoltages(void);

/**
 * Start an ADC conversion on the LTC6813
 * @return True if the ADC conversions have been sent out successfully. Else,
 * false
 */
bool Io_NewCellVoltages_StartAdcConversion(void);

/**
 * Get the location of the minimum cell voltage
 * @param segment The segment of the minimum cell voltage
 * @param cell The cell location of the minimum cell voltage
 */
void Io_NewCellVoltages_GetMinCellLocation(uint8_t *segment, uint8_t *cell);

/**
 * Get the location of the maximum cell voltage
 * @param segment The segment of the maximum cell voltage
 * @param cell The cell location of the maximum cell voltage
 */
void Io_NewCellVoltages_GetMaxCellLocation(uint8_t *segment, uint8_t *cell);

/**
 * Get the minimum cell voltage in (V)
 * @return The minimum cell voltage in (V)
 */
float Io_NewCellVoltages_GetMinCellVoltage(void);

/**
 * Get the maximum cell voltage in (V)
 * @return The maximum cell voltage in (V)
 */
float Io_NewCellVoltages_GetMaxCellVoltage(void);

/**
 * Get the pack voltage in (V)
 * @return The pack voltage in (V)
 */
float Io_NewCellVoltages_GetPackVoltage(void);

/**
 * Get the segment voltage in (V)
 * @param segment The segment to get the segment voltage from
 * @return The segment voltage in (V)
 */
float Io_NewCellVoltages_GetSegmentVoltage(AccumulatorSegments_E segment);

/**
 * Get the average cell voltage of all cells in the accumulator (V)
 * @return The average cell voltage of all cell in the accumulator (V)
 */
float Io_NewCellVoltages_GetAvgCellVoltage(void);
