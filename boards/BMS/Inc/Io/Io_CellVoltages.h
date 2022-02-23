#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "App_Accumulator.h"

bool Io_CellVoltages_GetAllRawCellVoltages(void);

bool Io_CellVoltages_StartAdcConversion(void);

void Io_CellVoltages_GetMinCellLocation(uint8_t *segment, uint8_t *cell);

void Io_CellVoltages_GetMaxCellLocation(uint8_t *segment, uint8_t *cell);

float Io_CellVoltages_GetMinCellVoltage(void);

float Io_CellVoltages_GetMaxCellVoltage(void);

float Io_CellVoltages_GetPackVoltage(void);

float Io_CellVoltages_GetSegmentVoltage(AccumulatorSegments_E segment);

float Io_CellVoltages_GetAvgCellVoltage(void);
