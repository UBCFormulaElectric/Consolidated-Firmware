#pragma once

#include "App_BmsWorld.h"

void App_SetPeriodicCanSignals_Imd(
    struct BmsCanTxInterface *can_tx,
    struct Imd *              imd);

void App_SetPeriodicSignals_CellMonitorInRangeChecks(
    struct BmsCanTxInterface *can_tx,
    const struct CellMonitor *cell_monitor);
