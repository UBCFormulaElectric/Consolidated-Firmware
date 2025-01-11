#include <stddef.h>
#include "io_fans.h"
#include "io_leds.h"
#include "app_mainState.h"
#include "app_coolant.h"
#include "app_loadCell.h"
#include "app_suspension.h"
#include "app_heartbeatMonitor.h"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
}

static void mainStateRunOnTick100Hz(void)()
{
    // add coolant broadcast
    // add suspension broadcast
    // app::loadCell::broadcast();
    // app_heartbeatMonitor_checkIn();
    // app_heartbeatMonitor_broadcastFaults();

    
    io::leds::brake_light_set(app_canRx_FSM_BrakeActuated_get());

    const bool hv_on = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    io::fans::acc_fan_set(hv_on);
    io::fans::rad_fan_set(hv_on);
}
namespace app::critstates
{
const app::State main_state{
    .name              = "MAIN",
    .run_on_entry      = NULL,
    .run_on_tick_1Hz   = NULL,
    .run_on_tick_100Hz = mainStateRunOnTick100Hz,
    .run_on_exit       = NULL,
};
}