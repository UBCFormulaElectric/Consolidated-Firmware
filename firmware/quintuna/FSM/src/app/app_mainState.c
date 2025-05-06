#include "app_mainState.h"
#include <stddef.h>
#include "app_apps.h"
#include "app_brake.h"
#include "app_shdnLoop.h"
#include "app_steering.h"
#include "app_suspension.h"

void mainStateRunOnTick100Hz(void)
{
    app_apps_broadcast();
    app_brake_broadcast();
    app_steering_broadcast();
    app_suspension_broadcast();
    app_shdnLoop_broadcast();
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
