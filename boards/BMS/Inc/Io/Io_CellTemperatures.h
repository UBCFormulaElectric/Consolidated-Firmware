#pragma once

#include "App_SharedExitCode.h"
#include <stdint.h>

/**
 * Read cell temperatures from all thermistors connected to the accumulator
 * @return EXIT_CODE_OK if cell temperatures (0.1°C) were acquired successfully
 * from all thermistors connected to the accumulator. Else, EXIT_CODE_ERROR
 */
ExitCode Io_CellTemperatures_ReadTemperatures(void);

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
