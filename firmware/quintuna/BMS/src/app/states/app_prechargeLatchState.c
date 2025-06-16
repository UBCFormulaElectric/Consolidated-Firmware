// TODO: Reset precharge limit exceeded
#include "states/app_allStates.h"
#include "states/app_initState.h"

#include "app_canTx.h"
#include "app_timer.h"

static TimerChannel precharge_latch_timer;
#define PRECHARGE_LATCH_TIMEOUT 3000U // 3 seconds

static void prechargeLatchStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_LATCH_STATE);

    app_timer_init(&precharge_latch_timer, PRECHARGE_LATCH_TIMEOUT);
    app_timer_restart(&precharge_latch_timer);
}

static void prechargeLatchStateRunOnTick100Hz(void)
{
    if (app_timer_updateAndGetState(&precharge_latch_timer) == TIMER_STATE_EXPIRED)
    {
        app_stateMachine_setNextState(app_initState_get());
    }

    // Run last since this checks for faults which overrides any other state transitions.
    app_allStates_runOnTick100Hz();
}

static void prechargeLatchStateRunOnExit(void) {}

const State *app_prechargeLatchState_get(void)
{
    static const State precharge_latch_state = {
        .name              = "PRECHARGE_LATCH",
        .run_on_entry      = prechargeLatchStateRunOnEntry,
        .run_on_tick_100Hz = prechargeLatchStateRunOnTick100Hz,
        .run_on_exit       = prechargeLatchStateRunOnExit,
    };

    return &precharge_latch_state;
}