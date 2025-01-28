#include <stddef.h>
#include "app_mainState.h"
#include "io_fans.h"
#include "io_leds.h"

void mainStateRunOnTick100Hz(void)
{
    const bool brake_light_on = false;
    // app_canRx_FSM_BrakeActuated_get();
    io_led_enable(&brake_light, brake_light_on);
    // app_canTx_RSM_BrakeLight_set(brake_light_on);
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