#include "states/App_AllStates.h"
#include "states/App_DriveState.h"
#include "App_SharedMacros.h"

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_DCM_Vitals_CurrentState_Set(DCM_INIT_STATE);

    // Disable inverters and apply zero torque upon entering init state
    App_CanTx_DCM_LeftInverterCommand_EnableInverter_Set(false);
    App_CanTx_DCM_RightInverterCommand_EnableInverter_Set(false);
    App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(0.0f);
    App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(0.0f);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
        // Initialize to true to prevent a false start
        static bool prev_start_switch_pos = true;

        const bool curr_start_switch_pos      = App_IsStartSwitchOn();
        const bool was_start_switch_pulled_up = !prev_start_switch_pos && curr_start_switch_pos;
        prev_start_switch_pos                 = curr_start_switch_pos;

        const bool is_brake_actuated = App_CanRx_FSM_Brake_IsActuated_Get();

        if (App_IsBmsInDriveState() && is_brake_actuated && was_start_switch_pulled_up)
        {
            // Transition to drive state when start-up conditions are passed (see
            // EV.10.4.3):
            App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
        }
    }
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState(void)
{
    static struct State init_state = {
        .name              = "INIT",
        .run_on_entry      = InitStateRunOnEntry,
        .run_on_tick_1Hz   = InitStateRunOnTick1Hz,
        .run_on_tick_100Hz = InitStateRunOnTick100Hz,
        .run_on_exit       = InitStateRunOnExit,
    };

    return &init_state;
}
