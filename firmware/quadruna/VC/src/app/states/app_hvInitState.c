#include "app_hvInitState.h"
#include "app_allStates.h"
#include "app_hvState.h"
#include "app_initState.h"

#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"

#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

static PowerStateConfig power_manager_hv_init = {
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
};

static void hvInitStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_HV_INIT_STATE);
    app_powerManager_updateConfig(power_manager_hv_init);

    // Enable inverters and apply zero torque upon entering HV Init state
    app_canTx_VC_LeftInverterEnable_set(true);
    app_canTx_VC_RightInverterEnable_set(true);
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    app_canTx_VC_LeftInverterTorqueLimit_set(0.0f);
    app_canTx_VC_RightInverterTorqueLimit_set(0.0f);
    app_canTx_VC_LeftInverterDirectionCommand_set(INVERTER_REVERSE_DIRECTION);
    app_canTx_VC_RightInverterDirectionCommand_set(INVERTER_FORWARD_DIRECTION);
}

static void hvInitStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void hvInitStateRunOnTick100Hz(void)
{
    const bool any_board_has_fault = app_faultCheck_checkBoards();
    const bool inverter_has_fault  = app_faultCheck_checkInverters();
    const bool all_states_ok       = !(any_board_has_fault || inverter_has_fault);

    const bool bms_in_drive = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;

    app_allStates_runOnTick100Hz();

    // TODO: stagger pump and aux (aux is always off)

    if (!bms_in_drive || inverter_has_fault)
    {
        app_stateMachine_setNextState(app_initState_get());
    }
    else if (all_states_ok)
    {
        app_stateMachine_setNextState(app_hvState_get());
    }
}

const State *app_hvInitState_get(void)
{
    static State hvInit_state = {
        .name              = "HV_INIT",
        .run_on_entry      = hvInitStateRunOnEntry,
        .run_on_tick_1Hz   = hvInitStateRunOnTick1Hz,
        .run_on_tick_100Hz = hvInitStateRunOnTick100Hz,
    };

    return &hvInit_state;
}
