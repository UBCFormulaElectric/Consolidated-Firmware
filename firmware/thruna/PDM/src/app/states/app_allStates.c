#include <stddef.h>
#include "states/app_allStates.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "app_globals.h"
#include "App_SharedMacros.h"
#include "App_SharedHeartbeatMonitor.h"
#include "io_lowVoltageBattery.h"

void app_allStates_runOnTick1Hz(struct StateMachine * const state_machine)
{
    UNUSED(state_machine);
}

void app_allStates_runOnTick100Hz(struct StateMachine * const state_machine)
{
    UNUSED(state_machine);

    App_CanTx_PDM_Heartbeat_Set(true);
    App_CanTx_PDM_BatVoltage_Set(io_lowVoltageBattery_getBatVoltage());
    App_CanTx_PDM_AccVoltage_Set(io_lowVoltageBattery_getAccVoltage());
    App_CanTx_PDM_BoostVoltage_Set(io_lowVoltageBattery_getBoostVoltage());

    App_SharedHeartbeatMonitor_CheckIn(globals->heartbeat_monitor);
    App_SharedHeartbeatMonitor_Tick(globals->heartbeat_monitor);
    App_SharedHeartbeatMonitor_BroadcastFaults(globals->heartbeat_monitor);
}
