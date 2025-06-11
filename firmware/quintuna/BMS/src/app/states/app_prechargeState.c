#include "app_states.h"

#include "app_timer.h"
#include "app_precharge.h"
#include "io_irs.h"

#include <assert.h>

static TimerChannel cooldown_timer;
#define PRECHARGE_COOLDOWN_TIME (1000U) // 1 second cooldown after precharge failure

static void prechargeStateRunOnEntry()
{
    io_irs_setPrecharge(IRS_CLOSED);
    app_precharge_restart();
}

static void prechargeStateRunOnTick100Hz()
{
    if (app_timer_updateAndGetState(&cooldown_timer) == TIMER_STATE_RUNNING)
    {
        return;
    }

    switch (app_precharge_poll(false))
    {
        case PRECHARGE_STATE_FAILED_CRITICAL: // precharge failed multiple times
            app_stateMachine_setNextState(&precharge_latch_state);
            break;
        case PRECHARGE_STATE_FAILED:
            app_timer_restart(&cooldown_timer);
            break;
        case PRECHARGE_STATE_SUCCESS:
            // Precharge successful, close positive contactor.
            app_stateMachine_setNextState(&charge_init_state);
            break;
        default:
            assert(0);
            break;
    }
}

static void prechargeStateRunOnExit()
{
    io_irs_setPrecharge(IRS_OPEN);
}

const State precharge_state = {
    .name              = "PRECHARGE",
    .run_on_entry      = prechargeStateRunOnEntry,
    .run_on_tick_100Hz = prechargeStateRunOnTick100Hz,
    .run_on_exit       = prechargeStateRunOnExit,
};