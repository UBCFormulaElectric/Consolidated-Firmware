#include <stddef.h>
#include "states/app_allStates.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "app_globals.h"
#include "app_utils.h"
#include "app_heartbeatMonitor.h"
#include "io_lowVoltageBattery.h"

void app_allStates_runOnTick100Hz(void)
{
    App_CanTx_PDM_Heartbeat_Set(true);
    App_CanTx_PDM_BatVoltage_Set(io_lowVoltageBattery_getBatVoltage());
    App_CanTx_PDM_AccVoltage_Set(io_lowVoltageBattery_getAccVoltage());
    App_CanTx_PDM_BoostVoltage_Set(io_lowVoltageBattery_getBoostVoltage());

    app_heartbeatMonitor_checkIn(globals->heartbeat_monitor);
    app_heartbeatMonitor_tick(globals->heartbeat_monitor);
    app_heartbeatMonitor_broadcastFaults(globals->heartbeat_monitor);
}
