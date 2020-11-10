#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "App_SharedExitCode.h"

/**
 * Read raw cell voltages from the cell monitoring chips.
 * @return EXIT_CODE_OK if the raw cell voltages (100µV) were acquired
 * successfully from all cell monitoring chips. Else, EXIT_CODE_ERROR.
 */
ExitCode Io_CellVoltages_ReadRawCellVoltages(void);

/**
 * Get the pointer to the 2D array containing raw cell voltages read from the
 * cell monitoring chips.
 * @param column_length The column length for the 2D array.
 * @note Call Io_CellVoltages_ReadCellVoltages to get the most recent raw cell
 * voltages from the cell monitoring chips before calling this function.
 * @return A pointer to the 2D array containing raw cell
 * voltages (100µV).
 */
uint16_t *Io_CellVoltages_GetRawCellVoltages(size_t *column_length);
