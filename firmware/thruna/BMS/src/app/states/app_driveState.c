#include "App_SharedMacros.h"
#include "app_airs.h"
#include "io_airs.h"
#include "states/app_allStates.h"

static void driveStateRunOnEntry(struct StateMachine* const state_machine)
{
    App_CanTx_BMS_State_Set(BMS_DRIVE_STATE);
}

static void driveStateRunOnTick1Hz(struct StateMachine* const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void driveStateRunOnTick100Hz(struct StateMachine* const state_machine)
{
    if (app_allStates_runOnTick100Hz(state_machine))
    {
        // if AIR- opens, go back to init state.
        const bool air_negative_opened = !io_airs_isNegativeClosed();
        if (air_negative_opened)
        {
            App_SharedStateMachine_SetNextState(state_machine, app_initState_get());
        }
    }
}

static void driveStateRunOnExit(struct StateMachine* const state_machine)
{
    UNUSED(state_machine);

    // AIR+ opens upon exiting drive state
    io_airs_openPositive();
}

const struct State* app_driveState_get(void)
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = driveStateRunOnExit,
    };

    return &drive_state;
}
