#include "states/App_DriveState.h"
#include "states/App_AllStates.h"
#include "App_SharedMacros.h"

void App_DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_FSM_Vitals_CurrentState_Set(FSM_DRIVE_STATE);
}

void App_DriveStateRunOnTick1Hz(struct StateMachine *state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}
void App_DriveStateRunOnTick100Hz(struct StateMachine *state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);
}
void App_DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name              = "DRIVE STATE",
        .run_on_entry      = App_DriveStateRunOnEntry,
        .run_on_tick_1Hz   = App_DriveStateRunOnTick1Hz,
        .run_on_tick_100Hz = App_DriveStateRunOnTick100Hz,
        .run_on_exit       = App_DriveStateRunOnExit,
    };
    return &drive_state;
}
