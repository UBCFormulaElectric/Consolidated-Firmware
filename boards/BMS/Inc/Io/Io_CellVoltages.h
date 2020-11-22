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

float Io_CellVoltages_GetMinCellVoltage(void);

float Io_CellVoltages_GetMaxCellVoltage(void);

float Io_CellVoltages_GetAverageCellVoltage(void);

float Io_CellVoltages_GetPackVoltage(void);

float Io_CellVoltages_GetSegment0Voltage(void);

float Io_CellVoltages_GetSegment1Voltage(void);

float Io_CellVoltages_GetSegment2Voltage(void);

float Io_CellVoltages_GetSegment3Voltage(void);

float Io_CellVoltages_GetSegment4Voltage(void);

float Io_CellVoltages_GetSegment5Voltage(void);
