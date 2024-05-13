#include "states/app_driveState.h"
#include "states/app_initState.h"
#include "app_powerManager.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "app_faultCheck.h"
#include <stddef.h>
#include "io_log.h"

static const PowerStateConfig power_manager_inverter_init = {
    .efuses = {
        [EFUSE_CHANNEL_SHDN] = true,
        [EFUSE_CHANNEL_LV] = true,
        [EFUSE_CHANNEL_PUMP] = true,
        [EFUSE_CHANNEL_AUX] = false,
        [EFUSE_CHANNEL_INV_R] = true,
        [EFUSE_CHANNEL_INV_L] = true,
        [EFUSE_CHANNEL_TELEM] = true,
        [EFUSE_CHANNEL_BUZZER] = false,
    },
    .pcm = true,
};

static bool prev_start_switch_pos;

static void inverterOnStateRunOnEntry(void)
{
    LOG_INFO("inverter on entry");
    app_canTx_VC_State_set(VC_INVERTER_ON_STATE);
    app_canTx_VC_LeftInverterEnable_set(false);
    app_canTx_VC_RightInverterEnable_set(false);
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    app_canTx_VC_LeftInverterTorqueLimit_set(0.0f);
    app_canTx_VC_RightInverterTorqueLimit_set(0.0f);
    app_powerManager_updateConfig(power_manager_inverter_init);

    prev_start_switch_pos = true;
    LOG_INFO("inverter on entry done");
}

static void inverterOnStateRunOnTick100Hz(void)
{
    const bool any_board_has_fault = app_boardFaultCheck();
    const bool inverter_has_fault  = app_inverterFaultCheck();
    const bool all_states_ok       = !(any_board_has_fault || inverter_has_fault);
    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    const bool curr_start_switch_on     = app_canRx_CRIT_StartSwitch_get();
    const bool was_start_switch_enabled = !prev_start_switch_pos && curr_start_switch_on;
    const bool is_brake_actuated        = app_canRx_FSM_BrakeActuated_get();

    if (!all_states_ok || app_canRx_BMS_State_get() != BMS_DRIVE_STATE)
    {
        LOG_INFO("Board Has Fault: %d, Inverters Has Fault: %d", any_board_has_fault, inverter_has_fault);
        LOG_ALL_FAULTS();
        app_stateMachine_setNextState(app_initState_get());
    }
    else if (is_brake_actuated && was_start_switch_enabled)
    {
        // Transition to drive state when start-up conditions are passed (see
        // EV.10.4.3):

        // TODO: Could not thoroughly validate VC refactor without a working BMS.
        // Thus, re-test IO, app, and vehicle dynamics before going HV up or driving again.
        app_stateMachine_setNextState(app_driveState_get());
    }

    // =========== OLD CODE ===========
    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    //    static bool prev_start_switch_pos = true;

    // TODO: Finish setting up DIM can set up once crit is done

    // const bool curr_start_switch_on      = app_canRx_DIM_StartSwitch_get();
    // const bool was_start_switch_enabled = !prev_start_switch_pos && curr_start_switch_on;
    // prev_start_switch_pos                 = curr_start_switch_on;

    // const bool bms_in_drive_state = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    // const bool is_brake_actuated  = app_canRx_FSM_BrakeActuated_get();

    // if (bms_in_drive_state && is_brake_actuated && was_start_switch_enabled && all_states_ok)
    // {
    //     // Transition to drive state when start-up conditions are passed (see
    //     // EV.10.4.3):

    //     // TODO: Could not thoroughly validate VC refactor without a working BMS.
    //     // Thus, re-test IO, app, and vehicle dynamics before going HV up or driving again.
    //     app_stateMachine_setNextState(app_driveState_get());
    // }
    prev_start_switch_pos = curr_start_switch_on;
}

static void inverterOnStateRunOnExit(void)
{
    LOG_INFO("inverter on exit");
}

const State *app_inverterOnState_get(void)
{
    static State inverter_on_state = {
        .name              = "INVERTER_ON",
        .run_on_entry      = inverterOnStateRunOnEntry,
        .run_on_tick_1Hz   = NULL,
        .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
        .run_on_exit       = inverterOnStateRunOnExit,
    };

    return &inverter_on_state;
}
