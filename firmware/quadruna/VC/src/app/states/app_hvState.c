#include "app_hvState.h"
#include "app_allStates.h"
#include "app_driveState.h"
#include "app_initState.h"

#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"

#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

static PowerStateConfig power_manager_hv = {
    .efuses = {
        [EFUSE_CHANNEL_SHDN] = true,
        [EFUSE_CHANNEL_LV] = true,
        [EFUSE_CHANNEL_PUMP] = true,
        [EFUSE_CHANNEL_AUX] = true,
        [EFUSE_CHANNEL_INV_R] = true,
        [EFUSE_CHANNEL_INV_L] = true,
        [EFUSE_CHANNEL_TELEM] = true,
        [EFUSE_CHANNEL_BUZZER] = false,
    },
};

static bool prev_start_switch_pos;

static void hvStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_HV_STATE);
    app_powerManager_updateConfig(power_manager_hv);

    // Disable inverters and apply zero torque upon entering init state
    app_canTx_VC_LeftInverterEnable_set(true);
    app_canTx_VC_RightInverterEnable_set(true);
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    app_canTx_VC_LeftInverterTorqueLimit_set(0.0f);
    app_canTx_VC_RightInverterTorqueLimit_set(0.0f);
    app_canTx_VC_LeftInverterDirectionCommand_set(INVERTER_REVERSE_DIRECTION);
    app_canTx_VC_RightInverterDirectionCommand_set(INVERTER_FORWARD_DIRECTION);

    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    prev_start_switch_pos = true;
}

static void hvStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void hvStateRunOnTick100Hz(void)
{
    const bool any_board_has_fault = app_faultCheck_checkBoards();
    const bool inverter_has_fault  = app_faultCheck_checkInverters();
    const bool all_states_ok       = !(any_board_has_fault || inverter_has_fault);

    const bool curr_start_switch_on     = app_canRx_CRIT_StartSwitch_get();
    const bool was_start_switch_enabled = !prev_start_switch_pos && curr_start_switch_on;
    const bool is_brake_actuated        = app_canRx_FSM_BrakeActuated_get();

    const bool bms_ready_for_drive = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    const bool bms_in_drive        = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;

    if (!bms_in_drive || inverter_has_fault)
    {
        app_stateMachine_setNextState(app_initState_get());
    }

    // Transition to drive state when start-up conditions are passed (see
    // EV.10.4.3):
    else if (all_states_ok && bms_ready_for_drive && is_brake_actuated && was_start_switch_enabled)
    {
        app_stateMachine_setNextState(app_driveState_get());
    }

    prev_start_switch_pos = curr_start_switch_on;
}

const State *app_hvState_get(void)
{
    static State hv_state = {
        .name              = "HV",
        .run_on_entry      = hvStateRunOnEntry,
        .run_on_tick_1Hz   = hvStateRunOnTick1Hz,
        .run_on_tick_100Hz = hvStateRunOnTick100Hz,
    };

    return &hv_state;
}
