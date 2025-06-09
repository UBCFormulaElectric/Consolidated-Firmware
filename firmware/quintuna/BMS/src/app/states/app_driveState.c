#include "jobs.h"
#include "app_canUtils.h"
#include "app_irs.h"
#include "app_canTx.h"
#include "app_initState.h"
#include "app_timer.h"

#include "io_irs.h"
#include "io_charger.h"

#define AIR_N_DEBOUNCE_PERIOD (200) // ms

static TimerChannel debounce_timer;

static void driveStateRunOnEntry(void)
{
    app_timer_init(&debounce_timer, AIR_N_DEBOUNCE_PERIOD);
    app_canTx_BMS_State_set(BMS_DRIVE_STATE);
}

static void driveStateRunOnTick1Hz(void) {}

static void driveStateRunOnTick100Hz(void)
{
    const bool        ir_negative_opened = !io_irs_isNegativeClosed();
    const EVSE_STATUS charger_status     = io_charger_getStatus();

    bool ir_negative_opened_debounced = false;

    if (app_timer_runIfCondition(&debounce_timer, ir_negative_opened) == TIMER_STATE_EXPIRED)
    {
        ir_negative_opened_debounced = true;
    }

    if (ir_negative_opened_debounced || charger_status != EVSE_DISCONNECTED)
    {
        app_stateMachine_setNextState(app_initState_get());
    }
}

static void driveStateRunOnExit(void)
{
    // IR+ opens upon exiting drive state
    io_irs_openPositive();
}

const State *app_driveState_get(void)
{
    static State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = driveStateRunOnExit,
    };

    return &drive_state;
}
