#include "states/app_allStates.h"
#include "app_utils.h"
#include "io_airs.h"
#include "app_airs.h"

static void driveStateRunOnEntry(void)
{
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
        if (air_negative_opened || is_charger_connected)
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
