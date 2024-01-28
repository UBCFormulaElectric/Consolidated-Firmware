#include <stddef.h>
#include "states/app_allStates.h"
#include "states/app_driveState.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "io_buzzer.h"
#include "app_globals.h"

static void initStateRunOnEntry(void)
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

static void initStateRunOnTick100Hz(void)
{
    const bool all_states_ok = app_allStates_runOnTick100Hz();

    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    static bool prev_start_switch_pos = true;

    const bool curr_start_switch_pos      = App_CanRx_DIM_StartSwitch_Get();
    const bool was_start_switch_pulled_up = !prev_start_switch_pos && curr_start_switch_pos;
    prev_start_switch_pos                 = curr_start_switch_pos;

    const bool bms_in_drive_state = App_CanRx_BMS_State_Get() == BMS_DRIVE_STATE;
    const bool is_brake_actuated  = App_CanRx_FSM_BrakeActuated_Get();

    if (bms_in_drive_state && is_brake_actuated && was_start_switch_pulled_up && all_states_ok)
    {
        // Transition to drive state when start-up conditions are passed (see
        // EV.10.4.3):

        // TODO: Could not thoroughly validate DCM refactor without a working BMS.
// Thus, re-test IO, app, and vehicle dynamics before going HV up or driving again.
#ifndef TARGET_EMBEDDED
        app_stateMachine_setNextState(app_driveState_get());
#endif
    }
}

const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = initStateRunOnEntry,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &init_state;
}
