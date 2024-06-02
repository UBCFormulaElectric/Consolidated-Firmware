#include "app_mainState.h"
#include <stddef.h>
#include "app_utils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_steering.h"
#include "app_wheels.h"
#include "app_apps.h"
#include "app_brake.h"
#include "app_suspension.h"
#include "app_loadCell.h"
#include "app_shdnLoop.h"

void mainStateRunOnTick100Hz(void)
{
    app_canTx_FSM_Heartbeat_set(true);

    app_apps_broadcast();
    app_brake_broadcast();
    app_steering_broadcast();
    app_wheels_broadcast();
    app_suspension_broadcast();
    app_loadCell_broadcast();
    app_shdn_loop_broadcast();
}

const State *app_mainState_get(void)
{
    static State main_state = {
        .name              = "MAIN STATE",
        .run_on_entry      = NULL,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = mainStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };
    return &main_state;
}
