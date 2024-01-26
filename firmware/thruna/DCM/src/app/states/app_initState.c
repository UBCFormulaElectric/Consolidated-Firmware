#include "states/app_allStates.h"
#include "states/app_driveState.h"
#include "App_SharedMacros.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "io_buzzer.h"
#include "app_globals.h"

static void initStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_DCM_State_Set(DCM_INIT_STATE);

    // Disable inverters and apply zero torque upon entering init state
    App_CanTx_DCM_LeftInverterEnable_Set(false);
    App_CanTx_DCM_RightInverterEnable_Set(false);
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(0.0f);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(0.0f);

    // Disable buzzer on transition to init.
    io_buzzer_enable(globals->config->buzzer, false);
    App_CanTx_DCM_BuzzerOn_Set(false);
}

static void initStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    app_allStates_runOnTick1Hz(state_machine);
}

static void initStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    const bool all_states_ok = app_allStates_runOnTick100Hz(state_machine);

    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    static bool prev_start_switch_pos = true;

    const bool curr_start_switch_pos      = App_CanRx_DIM_StartSwitch_Get() == SWITCH_ON;
    const bool was_start_switch_pulled_up = !prev_start_switch_pos && curr_start_switch_pos;
    prev_start_switch_pos                 = curr_start_switch_pos;

    const bool bms_in_drive_state = App_CanRx_BMS_State_Get() == BMS_DRIVE_STATE;
    const bool is_brake_actuated  = App_CanRx_FSM_BrakeActuated_Get();

    if (bms_in_drive_state && is_brake_actuated && was_start_switch_pulled_up && all_states_ok)
    {
        // Transition to drive state when start-up conditions are passed (see
        // EV.10.4.3):
        App_SharedStateMachine_SetNextState(state_machine, app_driveState_get());
    }
}

static void initStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *app_initState_get(void)
{
    static struct State init_state = {
        .name              = "INIT",
        .run_on_entry      = initStateRunOnEntry,
        .run_on_tick_1Hz   = initStateRunOnTick1Hz,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = initStateRunOnExit,
    };

    return &init_state;
}
