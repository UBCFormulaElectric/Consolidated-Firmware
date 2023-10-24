#include "states/App_FaultState.h"
#include "states/App_DriveState.h"
#include "states/App_AllStates.h"
#include "App_SharedMacros.h"

void App_FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
    App_CanTx_FSM_State_Set(FSM_STATE_FAULT);
    App_CanAlerts_FSM_StateMachineFault_Set(true);
}

void App_FaultStateRunOnTick1Hz(struct StateMachine *state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}
void App_FaultStateRunOnTick100Hz(struct StateMachine *state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    // Suppress accelerator pedal percentages (set to 0%)
    App_CanTx_FSM_PappsMappedPedalPercentage_Set(0);
    App_CanTx_FSM_SappsMappedPedalPercentage_Set(0);

    const bool hb_ok = !App_CanAlerts_FSM_MissingHeartbeatFault_Get();
    if (hb_ok)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
    }
}
void App_FaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
    App_CanAlerts_FSM_StateMachineFault_Set(false);
}

const struct State *App_GetFaultState(void)
{
    static struct State fault_state = {
        .name              = "FAULT STATE",
        .run_on_entry      = App_FaultStateRunOnEntry,
        .run_on_tick_1Hz   = App_FaultStateRunOnTick1Hz,
        .run_on_tick_100Hz = App_FaultStateRunOnTick100Hz,
        .run_on_exit       = App_FaultStateRunOnExit,
    };
    return &fault_state;
}
