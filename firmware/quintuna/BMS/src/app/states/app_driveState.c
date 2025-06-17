#include "app_stateMachine.h"
#include "app_timer.h"
#include "io_irs.h"
#include "app_canTx.h"
#include "states/app_allStates.h"
#include "states/app_initState.h"

#define AIR_N_DEBOUNCE_PERIOD (200) // ms

static TimerChannel debounce_timer;

static void driveStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_DRIVE_STATE);
    app_timer_init(&debounce_timer, AIR_N_DEBOUNCE_PERIOD);
}

static void driveStateRunOnTick100Hz(void)
{
    // if AIR- opens, go back to init state.
    const bool air_negative_opened = !io_irs_isNegativeClosed();
    const bool air_negative_opened_debounced =
        app_timer_runIfCondition(&debounce_timer, air_negative_opened) == TIMER_STATE_EXPIRED;

    if (air_negative_opened_debounced)
    {
        app_stateMachine_setNextState(app_initState_get());
    }

    // Run last since this checks for faults which overrides any other state transitions.
    app_allStates_runOnTick100Hz();
}

static void driveStateRunOnExit(void)
{
    // AIR+ opens upon exiting drive state
    io_irs_openPositive();
}

const State *app_driveState_get(void)
{
    static State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = driveStateRunOnExit,
    };

    return &drive_state;
}
