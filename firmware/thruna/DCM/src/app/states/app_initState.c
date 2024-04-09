#include <stddef.h>
#include "states/app_allStates.h"
#include "states/app_driveState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_buzzer.h"
#include "app_globals.h"

static void initStateRunOnEntry(void)
{
    app_canTx_DCM_State_set(DCM_INIT_STATE);

    // Disable inverters and apply zero torque upon entering init state
    app_canTx_DCM_LeftInverterEnable_set(false);
    app_canTx_DCM_RightInverterEnable_set(false);
    app_canTx_DCM_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_DCM_RightInverterTorqueCommand_set(0.0f);

    // Disable buzzer on transition to init.
    io_buzzer_enable(false);
    app_canTx_DCM_BuzzerOn_set(false);
}

static void initStateRunOnTick100Hz(void)
{
    const bool all_states_ok = app_allStates_runOnTick100Hz();

    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    static bool prev_start_switch_pos = true;

    const bool curr_start_switch_pos      = app_canRx_DIM_StartSwitch_get();
    const bool was_start_switch_pulled_up = !prev_start_switch_pos && curr_start_switch_pos;
    prev_start_switch_pos                 = curr_start_switch_pos;

    const bool bms_in_drive_state = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    const bool is_brake_actuated  = app_canRx_FSM_BrakeActuated_get();

    if (bms_in_drive_state && is_brake_actuated && was_start_switch_pulled_up && all_states_ok)
    {
        // Transition to drive state when start-up conditions are passed (see
        // EV.10.4.3):

        // TODO: Could not thoroughly validate DCM refactor without a working BMS.
        // Thus, re-test IO, app, and vehicle dynamics before going HV up or driving again.
        app_stateMachine_setNextState(app_driveState_get());
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
