#include <stddef.h>
#include "states/app_allStates.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "app_heartbeatMonitor.h"
#include "io_lowVoltageBattery.h"

void app_allStates_runOnTick1Hz(void) {}

void app_allStates_runOnTick100Hz(void)
{
    app_canTx_VC_BatVoltage_set(io_lowVoltageBattery_getBatVoltage());
    app_canTx_VC_AccVoltage_set(io_lowVoltageBattery_getAccVoltage());
    app_canTx_VC_BoostVoltage_set(io_lowVoltageBattery_getBoostVoltage());

    // TODO: add heartbeat monitor
    // app_heartbeatMonitor_checkIn();
    // app_heartbeatMonitor_tick();
    // app_heartbeatMonitor_broadcastFaults();
}
