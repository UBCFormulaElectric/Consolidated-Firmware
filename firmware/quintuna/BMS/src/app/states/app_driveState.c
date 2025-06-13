#include "states/app_states.h"

#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_timer.h"
#include "io_irs.h"

#define AIR_N_DEBOUNCE_PERIOD (200) // ms

static TimerChannel debounce_timer;

static void driveStateRunOnEntry(void)
{
    app_timer_init(&debounce_timer, AIR_N_DEBOUNCE_PERIOD);
    app_canTx_BMS_State_set(BMS_DRIVE_STATE);
    app_timer_init(&debounce_timer, AIR_N_DEBOUNCE_PERIOD);
}

static void driveStateRunOnTick100Hz(void)
{
    const bool                 ir_negative_opened = !io_irs_negativeState();

    // TODO move this logic to ALL STATES
    const bool ir_negative_opened_debounced = app_timer_runIfCondition(&debounce_timer, ir_negative_opened) == TIMER_STATE_EXPIRED;
    if (ir_negative_opened_debounced)
    {
        app_stateMachine_setNextState(&init_state);
    }
}

static void driveStateRunOnExit(void)
{
    // IR+ opens upon exiting drive state
    io_irs_setPositive(IRS_OPEN);
}

const State drive_state = {
    .name              = "DRIVE",
    .run_on_entry      = driveStateRunOnEntry,
    .run_on_tick_100Hz = driveStateRunOnTick100Hz,
    .run_on_exit       = driveStateRunOnExit,
};