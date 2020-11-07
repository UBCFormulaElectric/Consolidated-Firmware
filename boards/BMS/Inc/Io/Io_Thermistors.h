#pragma once

#include "App_SharedExitCode.h"

/**
 * Read the raw voltages across all thermistors connected to the cell monitoring
 * chips.
 * @return EXIT_CODE_OK if raw thermistor voltages (100µV) are read successfully
 * from all cell monitoring chips.
 */
ExitCode Io_Thermistors_ReadRawVoltages(void);

/**
 * Get the pointer to a 2D array containing raw voltages across all thermistors
 * for all cell monitoring chips.
 * @note Call Io_Thermistors_ReadRawVoltages to read the most recent thermistor
 * voltages from the cell monitoring chips before calling this function.
 * @note The row of the 2D array is equal to the number of cell monitoring
 * chips on the daisy chain. The column of the 2D array is equal to the number
 * of thermistor voltages that are monitored per cell monitoring chip.
 * @return A pointer to the 2D array containing raw voltages (100µV) across all
 * thermistors.
 */
uint16_t *Io_Thermistors_GetRawVoltages(void);
