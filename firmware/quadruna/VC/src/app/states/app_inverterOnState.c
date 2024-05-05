#include "states/app_driveState.h"
#include "states/app_allStates.h"
#include "states/app_initState.h"
#include "app_powerManager.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "app_canAlerts.h"
#include "app_faultCheck.h"
#include <stddef.h>
#include "io_log.h"
#include "io_time.h"

static void inverterOnStateRunOnEntry(void)
{
    LOG_INFO("Transitioning to INVERTER_ON state");
    app_canTx_VC_State_set(VC_INVERTER_ON_STATE);
}

static void inverterOnStateRunOnTick100Hz(void)
{
    const bool any_board_has_fault = app_boardFaultCheck();
    const bool inverter_has_fault  = app_inverterFaultCheck();
    const bool all_states_ok       = !(any_board_has_fault || inverter_has_fault);
    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    static bool prev_start_switch_pos = true;

    const bool curr_start_switch_pos      = app_canRx_CRIT_StartSwitch_get();
    const bool was_start_switch_pulled_up = !prev_start_switch_pos && curr_start_switch_pos;
    prev_start_switch_pos                 = curr_start_switch_pos;
    const bool is_brake_actuated          = app_canRx_FSM_BrakeActuated_get();
    const bool bms_in_drive_state         = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    const bool inverters_off_exit         = !all_states_ok;

    PowerManagerState nextState;
    if (app_canRx_BMS_State_get() == BMS_DRIVE_STATE)
        nextState = POWER_MANAGER_INVERTER_ON_POST_AIR_PLUS;
    else
        nextState = POWER_MANAGER_INVERTER_ON_PRE_AIR_PLUS;
    app_powerManager_setState(nextState);

    if (bms_in_drive_state && is_brake_actuated && was_start_switch_pulled_up && all_states_ok)
    {
        // Transition to drive state when start-up conditions are passed (see
        // EV.10.4.3):

        // TODO: Could not thoroughly validate VC refactor without a working BMS.
        // Thus, re-test IO, app, and vehicle dynamics before going HV up or driving again.
        LOG_INFO("Transitioning to DRIVE state");
        app_stateMachine_setNextState(app_driveState_get());
    }
    else if (inverters_off_exit)
    {
        LOG_INFO("Transitioning to INIT state");
        app_stateMachine_setNextState(app_initState_get());
    }

    // =========== OLD CODE ===========
    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    //    static bool prev_start_switch_pos = true;

    // TODO: Finish setting up DIM can set up once crit is done

    // const bool curr_start_switch_pos      = app_canRx_DIM_StartSwitch_get();
    // const bool was_start_switch_pulled_up = !prev_start_switch_pos && curr_start_switch_pos;
    // prev_start_switch_pos                 = curr_start_switch_pos;

    // const bool bms_in_drive_state = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    // const bool is_brake_actuated  = app_canRx_FSM_BrakeActuated_get();

    // if (bms_in_drive_state && is_brake_actuated && was_start_switch_pulled_up && all_states_ok)
    // {
    //     // Transition to drive state when start-up conditions are passed (see
    //     // EV.10.4.3):

    //     // TODO: Could not thoroughly validate VC refactor without a working BMS.
    //     // Thus, re-test IO, app, and vehicle dynamics before going HV up or driving again.
    //     app_stateMachine_setNextState(app_driveState_get());
    // }
}

const State *app_inverterOnState_get(void)
{
    static State inverter_on_state = {
        .name              = "INVERTER_ON",
        .run_on_entry      = inverterOnStateRunOnEntry,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
        .run_on_exit       = NULL,
    };

    return &inverter_on_state;
}
