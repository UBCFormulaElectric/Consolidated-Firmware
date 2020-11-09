#pragma once

#include <stdint.h>

/**
 * Initialize a set of functions used to calculate voltages.
 * @param get_cell_voltages A pointer to a 2D array containing raw cell voltages
 * measured by a daisy chain of cell monitoring chips.
 * @param num_of_cells_per_segment The number of cell voltage measurements per
 * segment.
 * @note The raw cell voltages are represented in 100µV. The raw voltages are
 * divided by 10000 to compute the voltage in V.
 */
void App_CellVoltages_Init(
    uint16_t *(*get_raw_cell_voltages)(void),
    uint32_t num_of_cells_per_segment);

/**
 * Get the average voltage for the 0th segment.
 * @return Get the average voltage for the 0th segment in V.
 */
float App_CellVoltages_GetSegment0Voltage(void);

/**
 * Get the average voltage for the 1st segment.
 * @return Get the average voltage for the 1st segment in V.
 */
float App_CellVoltages_GetSegment1Voltage(void);

/**
 * Get the average voltage for the 2nd segment.
 * @return Get the average voltage for the 2nd segment in V.
 */
float App_CellVoltages_GetSegment2Voltage(void);

/**
 * Get the average voltage for the 3rd segment.
 * @return The average voltage of the 3rd segment in V.
 */
float App_CellVoltages_GetSegment3Voltage(void);

/**
 * Get the average voltage for the 4th segment.
 * @return The average voltage of the 4th segment in V.
 */
float App_CellVoltages_GetSegment4Voltage(void);

/**
 * Get the average voltage for the 5th segment.
 * @return The average voltage of the 5th segment in V.
 */
float App_CellVoltages_GetSegment5Voltage(void);

/**
 * Get the pack voltage.
 * @return The pack voltage in V.
 */
float App_CellVoltages_GetPackVoltage(void);

/**
 * Get the average cell voltage.
 * @return The average cell voltage in V.
 */
float App_CellVoltages_GetAverageCellVoltage(void);

/**
 * Get the minimum cell voltage.
 * @return The minimum cell voltage in V.
 */
float App_CellVoltages_GetMinCellVoltage(void);

/**
 * Get the maximum cell voltage.
 * @return The maximum cell voltage in V.
 */
float App_CellVoltages_GetMaxCellVoltage(void);
