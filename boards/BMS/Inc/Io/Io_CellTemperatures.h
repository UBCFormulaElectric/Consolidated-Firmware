#pragma once

#include "App_SharedExitCode.h"

/**
 * Read the thermistor resistances from the cell monitoring chips.
 * @return EXIT_CODE_OK if all resistances (ohm) were acquired successfully from
 * all cell monitoring chips. Else, EXIT_CODE_ERROR.
 */
ExitCode Io_CellTemperatures_ReadTemperaturesDegC(void);

/**
 * Get thermistor resistances from the cell monitoring chips.
 * @return A pointer to a 2D array containing thermistor resistances (ohm).
 */
float *Io_CellTemperatures_GetTemperaturesDegC(void);
