#include "app_mainState.h"
#include "app_driveMode.h"
#include "app_shdnLoop.h"
#include "app_leds.h"
#include "app_switches.h"
#include "app_heartbeatMonitors.h"

static void mainStateRunOnTick100Hz(void)
{
    app_leds_update();
    app_switches_broadcast();
    app_driveMode_broadcast();

    app_shdnLoop_broadcast();

    app_heartbeatMonitor_checkIn(&hb_monitor);
    app_heartbeatMonitor_broadcastFaults(&hb_monitor);
}

static void mainStateRunOnEntry(void) {}
static void mainStateRunOnTick1Hz(void) {}
static void mainStateRunOnExit(void) {}

const State *app_mainState_get(void)
{
    static const State main_state = {
        .name              = "MAIN",
        .run_on_entry      = mainStateRunOnEntry,
        .run_on_tick_1Hz   = mainStateRunOnTick1Hz,
        .run_on_tick_100Hz = mainStateRunOnTick100Hz,
        .run_on_exit       = mainStateRunOnExit,
    };

    return &main_state;
}
