#pragma once

#include <stdint.h>
#include "App_Accumulator.h"

bool Io_CellVoltages_GetAllRawCellVoltages(void);

/**
 * Start an ADC conversion for all cell monitoring chips
 * @return True if ADC conversion request is sent out successfully. Else false
 */
bool Io_CellVoltages_StartAdcConversion(void);

/**
 * Read all voltages from cell monitoring chips
 * @return True if all cell voltages read back successfully. Else, false
 */
bool Io_CellVoltages_ReadVoltages(void);

/**
 * Get the min cell voltage location
 * @param segment The segment where the minimum cell is located
 * @param cell The cell where the minimum cell is located
 */
void Io_CellVoltages_GetMinCellLocation(uint8_t *segment, uint8_t *cell);

/**
 * Get the max cell voltage location
 * @param segment The segment where the maximum cell is located
 * @param cell The cell where the maximum cell is located
 */
void Io_CellVoltages_GetMaxCellLocation(uint8_t *segment, uint8_t *cell);

/**
 * Get the min cell voltage in V
 * @return The min cell voltage in V
 */
float Io_CellVoltages_GetMinCellVoltage(void);

/**
 * Get the max cell voltage in V
 * @return The max cell voltage in V
 */
float Io_CellVoltages_GetMaxCellVoltage(void);

/**
 * Get the battery pack voltage in V
 * @return The pack voltage in V
 */
float Io_CellVoltages_GetPackVoltage(void);

/**
 * Get the voltage of a given segment
 * @param The segment to obtain the segment voltage from
 * @return The voltage of the given segment in V
 **/
float Io_CellVoltages_GetSegmentVoltage(AccumulatorSegments_E segment);

/**
 * Get the average cell voltage of the battery pack
 * @return The average voltage of the battery pack in V
 */
float Io_CellVoltages_GetAvgCellVoltage(void);
