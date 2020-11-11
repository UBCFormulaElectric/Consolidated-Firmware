#pragma once

#include <stddef.h>
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
 * @param column_length The column length of the 2D array.
 * @note Call Io_CellTemperatures_ReadTemperaturesDegC to get the most recent
 * cell temperatures from the cell monitoring chips before calling this
 * function.
 */
float *Io_CellTemperatures_GetTemperaturesDegC(size_t *column_length);
