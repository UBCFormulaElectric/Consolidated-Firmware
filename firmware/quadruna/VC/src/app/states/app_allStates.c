#include "states/app_allStates.h"
// app
#include "app_sbgEllipse.h"
#include "app_canTx.h"
#include "app_heartbeatMonitor.h"
#include "app_lowVoltageBattery.h"
#include "app_currentSensing.h"
#include "app_efuse.h"
#include "app_pumpControl.h"
#include "app_shdnLoop.h"
#include "app_shdnLast.h"
// io
#include "io_sbgEllipse.h"

#define IGNORE_HEARTBEAT_CYCLES 3U

static uint16_t num_cycles = 0;

void app_allStates_runOnTick100Hz(void)
{
    app_lowVoltageBattery_broadcast();
    app_shdn_loop_broadcast();
    app_currentSensing_broadcast();
    app_efuse_broadcast();
    app_shdnLast_broadcast();

    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_tick();
    app_heartbeatMonitor_broadcastFaults();

    if (num_cycles <= IGNORE_HEARTBEAT_CYCLES)
    {
        num_cycles++;
    }

    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_tick();

    if (num_cycles > IGNORE_HEARTBEAT_CYCLES)
    {
        app_heartbeatMonitor_broadcastFaults();
    }
    io_sbgEllipse_handleLogs();
    app_sbgEllipse_broadcast();

    // Set status to false (which blocks drive) if either inverter is faulted, or another board has set a fault.
}

void app_allStates_runOnTick1Hz(void)
{
    app_canTx_VC_FlowRate_set(app_pumpControl_getFlowRate());
}
