#pragma once

#include <stddef.h>

/**
 * Initialize a set of functions used to get internal die temperatures for the
 * accumulator.
 * @param get_die_temperatures A function that returns a pointer to an array
 * @param get_cell_temperatures A function that returns a pointer to a 2D array
 * containing cell temperatures. containing internal die temperatures measured
 * from all cell monitors.
 */
void App_AccumulatorTemperatures_Init(
    float *(*get_die_temperatures)(void),
    float *(*get_cell_temperatures)(size_t *));

/**
 * Get the internal die temperature of the cell monitoring chip for the 0th
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 0th accumulator segment.
 */
float App_AccumulatorTemperatures_GetSegment0DieTemperatureDegC(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 1st
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 1st accumulator segment.
 */
float App_AccumulatorTemperatures_GetSegment1DieTemperatureDegC(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 2nd
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 2nd accumulator segment.
 */
float App_AccumulatorTemperatures_GetSegment2DieTemperatureDegC(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 3rd
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 3rd accumulator segment.
 */
float App_AccumulatorTemperatures_GetSegment3DieTemperatureDegC(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 4th
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 4th accumulator segment.
 */
float App_AccumulatorTemperatures_GetSegment4DieTemperatureDegC(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 5th
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 5th accumulator segment.
 */
float App_AccumulatorTemperatures_GetSegment5DieTemperatureDegC(void);

/**
 * Get the minimum internal die temperature.
 * @return The minimum internal die temperature (°C).
 */
float App_AccumulatorTemperatures_GetMinDieTemperatureDegC(void);

/**
 *
 * Get the maximum internal die temperature.
 * @return The maximum internal die temperature (°C).
 */
float App_AccumulatorTemperatures_GetMaxDieTemperatureDegC(void);

/**
 * Get the minimum accumulator cell temperature.
 * @return The minimum accumulator cell temperature (°C).
 */
float App_AccumulatorTemperatures_GetMinCellTemperatureDegC(void);

/**
 * Get the maximum accumulator cell temperature.
 * @return The maximum accumulator cell temperature (°C).
 */
float App_AccumulatorTemperatures_GetMaxCellTemperatureDegC(void);

/**
 * Get the average accumulator cell temperature.
 * @return The average accumulator cell temperature (°C).
 */
float App_AccumulatorTemperatures_GetAverageCellTemperatureDegC(void);
