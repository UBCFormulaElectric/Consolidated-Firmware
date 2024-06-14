#include "states/app_allStates.h"
#include "app_utils.h"
#include "io_airs.h"
#include "app_airs.h"

#define AIR_N_DEBOUNCE_PERIOD (200) // ms

static TimerChannel debounce_timer;

static void driveStateRunOnEntry(void)
{
    app_timer_init(&debounce_timer, AIR_N_DEBOUNCE_PERIOD);
    app_canTx_BMS_State_set(BMS_DRIVE_STATE);
}

static void driveStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void driveStateRunOnTick100Hz(void)
{
    if (app_allStates_runOnTick100Hz())
    {
        // if AIR- opens, go back to init state.
        const bool air_negative_opened  = !io_airs_isNegativeClosed();
        const bool is_charger_connected = app_canRx_BRUSA_IsConnected_get();

        bool air_negative_opened_debounced = false;

        // app_timer_runIfCondition(&debounce_timer, air_negative_opened);

        if (app_timer_runIfCondition(&debounce_timer, air_negative_opened) == TIMER_STATE_EXPIRED)
        {
            air_negative_opened_debounced = true;
        }

        if (air_negative_opened_debounced || is_charger_connected)
        {
            app_stateMachine_setNextState(app_initState_get());
        }
    }
}

static void driveStateRunOnExit(void)
{
    // AIR+ opens upon exiting drive state
    io_airs_openPositive();
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