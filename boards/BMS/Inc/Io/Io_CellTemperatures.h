#pragma once

/**
 * Read raw thermistor voltages from the cell monitoring chips
 * @return EXIT_CODE_OK if raw thermistor voltages are read successfully from
 * all cell monitoring chips. Else, EXIT_CODE_ERROR
 */
bool Io_CellTemperatures_GetCellTemperatureDegC(void);

/**
 * Get the current minimum accumulator cell temperature out of all cell
 * temperatures
 * @return The current minimum cell temperature (0.1°C)
 */
uint32_t Io_CellTemperatures_GetMinCellTemperature(void);

/**
 * Get the current maximum accumulator cell temperature out of all cell
 * temperatures
 * @return The current maximum cell temperature (0.1°C)
 */
uint32_t Io_CellTemperatures_GetMaxCellTemperature(void);

/**
 * Get the average accumulator cell temperature
 * @return The average accumulator cell temperature (0.1°C)
 */
float Io_CellTemperatures_GetAverageCellTemperature(void);
