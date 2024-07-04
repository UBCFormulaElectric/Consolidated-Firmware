#include "app_mainState.h"
#include "app_switches.h"
#include "app_leds.h"
#include "app_driveMode.h"
#include "app_critShdn.h"
#include "app_critHeartbeat.h"

static void mainStateRunOnTick100Hz()
{
    app::switches::broadcast();
    app::leds::update();
    app::driveMode::broadcast();
    app::shdn::critShdn.broadcast();
    app::heartbeat::critHeartbeat.checkIn();
    app::heartbeat::critHeartbeat.broadcastFaults();
}
static void mainStateRunOnEntry() {}
static void mainStateRunOnTick1Hz() {}
static void mainStateRunOnExit() {}

const State main_state{
    .name              = "MAIN",
    .run_on_entry      = mainStateRunOnEntry,
    .run_on_tick_1Hz   = mainStateRunOnTick1Hz,
    .run_on_tick_100Hz = mainStateRunOnTick100Hz,
    .run_on_exit       = mainStateRunOnExit,
};