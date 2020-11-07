#pragma once

#include "App_SharedExitCode.h"

/**
 * Read the cell temperatures from all cell monitoring chips.
 * @return EXIT_CODE_OK if all temperatures (°C) were acquired successfully from
 * all cell monitoring chips. Else, EXIT_CODE_ERROR.
 */
ExitCode Io_CellTemperatures_ReadTemperaturesDegC(void);

/**
 * Get the pointer to the 2D array containing cell temperatures read from all
 * cell monitoring chips.
 * @note Call Io_CellTemperatures_ReadTemperaturesDegC to get the most recent
 * cell temperatures from the cell monitoring chips before calling this
 * function.
 * @note The row of the 2D array is equal to the number of cell monitoring
 * devices on the daisy chain. The column of the 2D array is equal to the number
 * of cell temperatures that are monitored per cell monitoring chip.
 * @return A pointer to the 2D array containing converted cell temperatures
 * (°C).
 */
float *Io_CellTemperatures_GetTemperaturesDegC(void);
