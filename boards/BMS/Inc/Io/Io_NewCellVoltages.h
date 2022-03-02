#pragma once

#include "App_Accumulator.h"

bool Io_NewCellVoltages_ReadVoltages(void);

bool Io_NewCellVoltages_StartAdcConversion(void);

void Io_NewCellVoltages_GetMinCellLocation(uint8_t *segment, uint8_t *cell);

void Io_NewCellVoltages_GetMaxCellLocation(uint8_t *segment, uint8_t *cell);

float Io_NewCellVoltages_GetMinCellVoltage(void);

float Io_NewCellVoltages_GetMaxCellVoltage(void);

float Io_NewCellVoltages_GetPackVoltage(void);

float Io_NewCellVoltages_GetSegmentVoltage(AccumulatorSegments_E segment);

float Io_NewCellVoltages_GetAvgCellVoltage(void);
