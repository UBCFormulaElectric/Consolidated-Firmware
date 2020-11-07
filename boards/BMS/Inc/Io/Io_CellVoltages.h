#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "App_SharedExitCode.h"

/**
 * Read cell voltages from the cell monitoring chips.
 * @return EXIT_CODE_OK if cell voltages (100µV) were acquired successfully from
 * all cell monitoring chips. Else, EXIT_CODE_ERROR.
 */
ExitCode Io_CellVoltages_ReadCellVoltages(void);

/**
 * Get the pointer to the 2D array containing cell voltages read from the cell
 * monitoring chips.
 * @note Call Io_CellVoltages_ReadCellVoltages to get the most recent cell
 * voltages from the cell monitoring chips before calling this function.
 * @note The row of the 2D array is equal to the number of cell monitoring
 * devices on the daisy chain. The column of the 2D array is equal to the number
 * of cell voltages that are monitored per cell monitoring chip.
 * @return A pointer to the 2D array containing converted cell
 * voltages (100µV).
 */
uint16_t *Io_CellVoltages_GetCellVoltages(void);
