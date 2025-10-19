#include "states/app_allStates.h"
#include "states/app_prechargeState.h"
#include "states/app_inverterOnState.h"
#include "utils.h"
#include "app_timer.h"

static TimerChannel timer;

static void inverterOnStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_INVERTER_ON_STATE);
    app_timer_init(&timer, INVERTER_BOOTUP_TIME_MS);
    app_timer_restart(&timer);
}

static void inverterOnStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void inverterOnStateRunOnTick100Hz(void)
{
    if (app_allStates_runOnTick100Hz())
    {
        TimerState timer_state = app_timer_updateAndGetState(&timer);

        if (timer_state == TIMER_STATE_EXPIRED)
        {
            app_stateMachine_setNextState(app_prechargeState_get());
        }
    }
}

const State *app_inverterOnState_get(void)
{
    static State inverter_state = {
        .name              = "INVERTER_ON",
        .run_on_entry      = inverterOnStateRunOnEntry,
        .run_on_tick_1Hz   = inverterOnStateRunOnTick1Hz,
        .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &inverter_state;
}
