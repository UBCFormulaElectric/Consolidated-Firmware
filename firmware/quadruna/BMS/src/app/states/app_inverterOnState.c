#include "states/app_allStates.h"
#include "states/app_prechargeState.h"
#include "states/app_inverterOnState.h"
#include "app_utils.h"
#include "app_timer.h"

#define CHARGING_MILLISECONDS 200

static TimerChannel timer;
static bool         has_time_passed;

static void inverterOnStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_INVERTER_ON_STATE);
    app_timer_init(&timer, CHARGING_MILLISECONDS);
    app_timer_restart(&timer);
}

void app_inverterOnState_init()
{
    has_time_passed = false;
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

        if (timer_state == TIMER_STATE_EXPIRED || has_time_passed)
        {
            app_stateMachine_setNextState(app_prechargeState_get());
            has_time_passed = true;
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
