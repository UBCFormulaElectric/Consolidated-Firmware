#include "app_inverterOnState.h"
#include "app_allStates.h"
#include "app_pcmState.h"

#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"

#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

static PowerStateConfig power_manager_inverter_on = {
    .efuses = {
        [EFUSE_CHANNEL_SHDN] = true,
        [EFUSE_CHANNEL_LV] = true,
        [EFUSE_CHANNEL_PUMP] = false,
        [EFUSE_CHANNEL_AUX] = false,
        [EFUSE_CHANNEL_INV_R] = false,
        [EFUSE_CHANNEL_INV_L] = false,
        [EFUSE_CHANNEL_TELEM] = true,
        [EFUSE_CHANNEL_BUZZER] = false,
    },
};

static void inverterOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_INVERTER_ON_STATE);
    app_powerManager_updateConfig(power_manager_inverter_on);

    // Disable inverters and apply zero torque upon entering inverterOn state
    app_canTx_VC_LeftInverterEnable_set(false);
    app_canTx_VC_RightInverterEnable_set(false);
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    app_canTx_VC_LeftInverterTorqueLimit_set(0.0f);
    app_canTx_VC_RightInverterTorqueLimit_set(0.0f);
    app_canTx_VC_LeftInverterDirectionCommand_set(INVERTER_REVERSE_DIRECTION);
    app_canTx_VC_RightInverterDirectionCommand_set(INVERTER_FORWARD_DIRECTION);
}

static void inverterOnStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void inverterOnStateRunOnTick100Hz(void)
{
    const bool any_board_has_fault = app_faultCheck_checkBoards();
    const bool inverter_has_fault  = app_faultCheck_checkInverters();
    const bool all_states_ok       = !(any_board_has_fault || inverter_has_fault);

    const bool bms_in_drive = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;

    const bool is_invr_on = app_canRx_INVL_VsmState_get() == INVERTER_VSM_READY_STATE;
    const bool is_invl_on = app_canRx_INVL_VsmState_get() == INVERTER_VSM_READY_STATE;
    const bool is_inv_on  = is_invr_on && is_invl_on;

    if (!power_manager_inverter_on.efuses[EFUSE_CHANNEL_INV_R])
    {
        io_efuse_setChannel(EFUSE_CHANNEL_INV_R, true);
        power_manager_inverter_on.efuses[EFUSE_CHANNEL_INV_R] = true;
        app_canTx_VC_RightInverterEnable_set(true);
    }
    else if (!power_manager_inverter_on.efuses[EFUSE_CHANNEL_INV_L])
    {
        io_efuse_setChannel(EFUSE_CHANNEL_INV_L, true);
        power_manager_inverter_on.efuses[EFUSE_CHANNEL_INV_L] = true;
        app_canTx_VC_LeftInverterEnable_set(true);
    }
    else if (is_inv_on)
    {
        app_canTx_VC_isPrechargeReady_set(true);
    }

    if (all_states_ok && bms_in_drive && is_inv_on)
    {
        app_stateMachine_setNextState(app_pcmState_get());
    }
}

const State *app_inverterOnState_get(void)
{
    static State inverterOn_state = {
        .name              = "INVERTER_ON",
        .run_on_entry      = inverterOnStateRunOnEntry,
        .run_on_tick_1Hz   = inverterOnStateRunOnTick1Hz,
        .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
    };

    return &inverterOn_state;
}
