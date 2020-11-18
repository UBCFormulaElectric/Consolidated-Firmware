#pragma once

#include "App_SharedExitCode.h"

/**
 * Read internal die temperatures (°C) for all cell monitoring chips.
 * @return EXIT_CODE_OK if internal die temperatures were acquired successfully
 * from all cell monitoring chips. Else, EXIT_CODE_ERROR.
 */
ExitCode Io_Temperatures_ReadDieTemperaturesDegC(void);

/**
 * Get the pointer to the array containing internal die temperatures read
 * from the cell monitoring chips.
 * @note The length of the array is equal to the number of cell monitoring
 * devices on the daisy chain.
 * @note Call Io_Temperatures_ReadDieTemperatures to acquire the most recent
 * die temperatures from the cell monitoring chips before calling this function.
 * @return A pointer to the array containing the internal die temperatures (°C)
 * for each cell monitoring chip
 */
float *Io_Temperatures_GetDieTemperaturesDegC(void);
