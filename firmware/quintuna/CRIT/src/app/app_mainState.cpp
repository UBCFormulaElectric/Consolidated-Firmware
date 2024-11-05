#include "app_mainState.h"
#include "app_switches.h"
#include "app_leds.h"
#include "app_driveMode.h"
#include "app_critShdn.h"
#include "app_critHeartbeat.h"
#include "app_stackMonitorConfig.h"

static void mainStateRunOnTick100Hz()
{
    app::switches::broadcast();
    app::leds::update();
    app::driveMode::broadcast();
    app::shdn::critShdn.broadcast();
    app::heartbeat::critHeartbeat.checkIn();
    app::heartbeat::critHeartbeat.broadcastFaults();
    app::crit_stack_monitor.check();
}
static void mainStateRunOnEntry() {}
static void mainStateRunOnTick1Hz() {}
static void mainStateRunOnExit() {}

namespace app::critstates
{
const app::State main_state{
    .name              = "MAIN",
    .run_on_entry      = mainStateRunOnEntry,
    .run_on_tick_1Hz   = mainStateRunOnTick1Hz,
    .run_on_tick_100Hz = mainStateRunOnTick100Hz,
    .run_on_exit       = mainStateRunOnExit,
};
}
