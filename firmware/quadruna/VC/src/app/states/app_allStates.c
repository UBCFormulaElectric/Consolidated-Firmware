#include <stddef.h>
#include "states/app_allStates.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "app_heartbeatMonitor.h"
#include "app_lowVoltageBattery.h"

void app_allStates_runOnTick1Hz(void) {}

void app_allStates_runOnTick100Hz(void)
{
    app_lowVoltageBattery_broadcast();
    
    // TODO: add heartbeat monitor
    // app_heartbeatMonitor_checkIn();
    // app_heartbeatMonitor_tick();
    // app_heartbeatMonitor_broadcastFaults();
}
