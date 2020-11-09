#pragma once

#include <stdint.h>

/**
 * Initialize a set of functions used to calculate voltages for the accumulator.
 * @param get_cell_voltages A pointer to a 2D array containing raw cell voltages
 * measured from all cell monitors.
 * @param num_of_cells_per_segment The number of cell voltages measured per
 * segment.
 * @note The raw cell voltages are represented in 100µV. The raw voltages are
 * divided by 10000 to compute voltages in V.
 */
void App_AccumulatorVoltages_Init(
    uint16_t *(*get_raw_cell_voltages)(void),
    uint32_t num_of_cells_per_segment);

/**
 * Get the average voltage for the 0th accumulator segment.
 * @return Get the average voltage for the 0th accumulator segment in V.
 */
float App_AccumulatorVoltages_GetSegment0Voltage(void);

/**
 * Get the average voltage for the 1st accumulator segment.
 * @return Get the average voltage for the 1st accumulator segment in V.
 */
float App_AccumulatorVoltages_GetSegment1Voltage(void);

/**
 * Get the average voltage for the 2nd accumulator segment.
 * @return Get the average voltage for the 2nd accumulator segment in V.
 */
float App_AccumulatorVoltages_GetSegment2Voltage(void);

/**
 * Get the average voltage for the 3rd accumulator segment.
 * @return The average voltage of the 3rd accumulator segment in V.
 */
float App_AccumulatorVoltages_GetSegment3Voltage(void);

/**
 * Get the average voltage for the 4th accumulator segment.
 * @return The average voltage of the 4th accumulator segment in V.
 */
float App_AccumulatorVoltages_GetSegment4Voltage(void);

/**
 * Get the average voltage for the 5th accumulator segment.
 * @return The average voltage of the 5th accumulator segment in V.
 */
float App_AccumulatorVoltages_GetSegment5Voltage(void);

/**
 * Get the pack voltage for the accumulator.
 * @return The pack voltage for the accumulator in V.
 */
float App_AccumulatorVoltages_GetPackVoltage(void);

/**
 * Get the average cell voltage for the accumulator.
 * @return The average accumulator cell voltage in V.
 */
float App_AccumulatorVoltages_GetAverageCellVoltage(void);

/**
 * Get the minimum cell voltage for the accumulator.
 * @return The minimum accumulator cell voltage in V.
 */
float App_AccumulatorVoltages_GetMinCellVoltage(void);

/**
 * Get the maximum cell voltage for the accumulator.
 * @return The maximum accumulator cell voltage in V.
 */
float App_AccumulatorVoltages_GetMaxCellVoltage(void);
