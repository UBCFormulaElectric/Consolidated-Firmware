#include "states/app_states.h"

#include "app_precharge.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_timer.h"
#include "io_irs.h"

#include <assert.h>

static TimerChannel cooldown_timer;
#define PRECHARGE_COOLDOWN_TIME (1000U) // 1 second cooldown after precharge failure

static void app_prechargeDriveStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_DRIVE_STATE);

    io_irs_setPrecharge(IRS_CLOSED);
    app_precharge_restart();

    app_timer_init(&cooldown_timer, PRECHARGE_COOLDOWN_TIME);
}

static void app_prechargeDriveStateRunOnTick100Hz(void)
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
            io_irs_setPositive(IRS_CLOSED); // i am not sure this is smart? cause if all states overrides into fault
                                            // state, does it close contactors in time?
            // also on the other hand you want to close positive before you open precharge, so i guess?? but i think the
            // time between the two should be very minimal since they get handled in the onentry and onexit of each
            // state transition, which happens consecutively
            app_stateMachine_setNextState(&drive_state);
            break;
        default:
            assert(0);
            break;
    }
}

static void app_prechargeDriveStateRunOnExit(void)
{
    io_irs_setPrecharge(IRS_OPEN);
}

const State precharge_drive_state = {
    .name              = "PRECHARGE DRIVE",
    .run_on_entry      = app_prechargeDriveStateRunOnEntry,
    .run_on_tick_100Hz = app_prechargeDriveStateRunOnTick100Hz,
    .run_on_exit       = app_prechargeDriveStateRunOnExit,
};
