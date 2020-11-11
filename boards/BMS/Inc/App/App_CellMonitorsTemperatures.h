#pragma once

/**
 * Initialize a set of functions to get internal die temperatures for the cell
 * monitoring chips.
 */
void App_CellMonitorTemperatures_Init(float *(*get_die_temperatures)(void));

/**
 * Get the internal die temperature of the cell monitoring chip corresponding to
 * the 0th accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip
 * corresponding to the 0th accumulator segment.
 */
float App_CellMonitorsTemperatures_GetDieTemp0DegC(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 1st
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 1st accumulator segment.
 */
float App_CellMonitorsTemperatures_GetDieTemp1DegC(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 2nd
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 2nd accumulator segment.
 */
float App_CellMonitorsTemperatures_GetDieTemp2DegC(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 3rd
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 3rd accumulator segment.
 */
float App_CellMonitorsTemperatures_GetDieTemp3DegC(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 4th
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 4th accumulator segment.
 */
float App_CellMonitorsTemperatures_GetDieTemp4DegC(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 5th
 * accumulator segment.
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 5th accumulator segment.
 */
float App_CellMonitorsTemperatures_GetDieTemp5DegC(void);

/**
 * Get the minimum internal die temperature.
 * @return The minimum internal die temperature (°C).
 */
float App_CellMonitorsTemperatures_GetMinDieTempDegC(void);

/**
 * Get the maximum internal die temperature.
 * @return The maximum internal die temperature (°C).
 */
float App_CellMonitorsTemperatures_GetMaxDieTempDegC(void);
