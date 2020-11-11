#pragma once

#include <stddef.h>

/**
 * Initialize a set of functions used to get accumulator cell temperatures.
 * @param get_cell_temperatures A function that returns a pointer to a 2D array
 * containing cell temperatures. containing internal die temperatures measured
 * from all cell monitors.
 */
void App_AccumulatorTemperatures_Init(
    float *(*get_cell_temperatures)(size_t *));

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
