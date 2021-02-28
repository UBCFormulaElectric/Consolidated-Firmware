#pragma once

#include "App_BmsWorld.h"

void App_SetPeriodicCanSignals_Imd(
    struct BmsCanTxInterface *can_tx,
    struct Imd *              imd);

void App_SetPeriodicSignals_AccumulatorVoltagesInRangeChecks(
    struct BmsCanTxInterface *can_tx,
    const struct Accumulator *accumulator);

void App_SetPeriodicSignals_AccumulatorTemperaturesInRangeChecks(
    struct BmsCanTxInterface *can_tx,
    const struct Accumulator *accumulator);

void App_SetPeriodicSignals_CellMonitorsInRangeChecks(
    struct BmsCanTxInterface * can_tx,
    const struct CellMonitors *cell_monitors);
