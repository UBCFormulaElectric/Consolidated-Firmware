#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "App_SharedExitCode.h"
#include "configs/App_SharedConstants.h"

/**
 * Read raw cell voltages from the cell monitoring chips.
 * @return EXIT_CODE_OK if the raw cell voltages (100µV) were acquired
 * successfully from all cell monitoring chips. Else, EXIT_CODE_ERROR.
 */
bool Io_CellVoltages_GetAllRawCellVoltages(void);

void Io_CellVoltages_GetMinCellLocation(uint8_t *segment, uint8_t *cell);

void Io_CellVoltages_GetMaxCellLocation(uint8_t *segment, uint8_t *cell);

float Io_CellVoltages_GetMinCellVoltage(void);

float Io_CellVoltages_GetMaxCellVoltage(void);

float Io_CellVoltages_GetPackVoltage(void);

float Io_CellVoltages_GetSegmentVoltage(enum AccumulatorSegments ic);

float Io_CellVoltages_GetAvgCellVoltage(void);
