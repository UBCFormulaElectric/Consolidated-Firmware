#pragma once

#include "App_SharedExitCode.h"

/**
 * Read internal die temperatures for all cell monitoring chips.
 * @return EXIT_CODE_OK if internal die temperatures were acquired successfully
 * from all cell monitoring chips. Else, EXIT_CODE_ERROR.
 */
ExitCode Io_Temperatures_ReadDieTemperatures(void);

/**
 * Get the pointer to the array containing internal die temperatures read
 * from the cell monitoring chips.
 * @note The subscript value of the array is the number of cell monitoring
 * devices on the daisy chain.
 * @note Call Io_Temperatures_ReadDieTemperatures to acquire the most recent
 * die temperatures from the cell monitoring chips before calling this function.
 * @note The internal die temperature is read back from the cell monitoring
 * chips as a voltage. Use the following equation to convert voltages to
 * temperatures (°C):
 *
 * ITMP(Measured_V) = Measured_V * (1°C / 13.1578e-3V) - 276°C
 *
 * @return A pointer to the array containing the internal die temperatures for
 * each cell monitoring chip.
 */
uint16_t *Io_Temperatures_GetDieTemperatures(void);
