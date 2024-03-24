#include <stddef.h>
#include "states/app_allStates.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "app_heartbeatMonitor.h"
#include "app_lowVoltageBattery.h"
#include "app_shutdown.h"
#include "app_currentSensing.h"
#include "app_efuse.h"

void app_allStates_runOnTick1Hz(void) {}

void app_allStates_runOnTick100Hz(void)
{
    app_lowVoltageBattery_broadcast();
    app_shutdown_broadcast();
    app_currentSensing_broadcast();
    app_efuse_broadcast();

    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_tick();
    app_heartbeatMonitor_broadcastFaults();
}
