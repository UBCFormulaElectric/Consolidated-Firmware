// states
#include "app_states.h"
// jsoncan
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"

#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"
#include "io_tsms.h"
#include "io_log.h"

static const PowerStateConfig power_manager_shutdown_init = {
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
    .pcm = false,
};

static void initStateRunOnEntry(void)
{
    LOG_INFO("init entry");
    app_canTx_VC_State_set(VC_INIT_STATE);
    app_powerManager_updateConfig(power_manager_shutdown_init);
    // Disable inverters and apply zero torque upon entering init state
    app_canTx_VC_LeftInverterEnable_set(false);
    app_canTx_VC_RightInverterEnable_set(false);
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    app_canTx_VC_LeftInverterTorqueLimit_set(0.0f);
    app_canTx_VC_RightInverterTorqueLimit_set(0.0f);
    app_canTx_VC_LeftInverterDirectionCommand_set(INVERTER_REVERSE_DIRECTION);
    app_canTx_VC_RightInverterDirectionCommand_set(INVERTER_FORWARD_DIRECTION);

    // Disable buzzer on transition to init.
    app_powerManager_updateEfuse(EFUSE_CHANNEL_BUZZER, false);
    LOG_INFO("init entry done");
}

static void initStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();
    const bool any_board_has_fault = app_boardFaultCheck();
    const bool inverter_has_fault  = app_inverterFaultCheck();
    const bool all_states_ok       = !(any_board_has_fault || inverter_has_fault);

    const bool is_hv_key_turned = io_tsms_read();

    if (app_canRx_BMS_State_get() == BMS_DRIVE_STATE && is_hv_key_turned && all_states_ok) // just as a sanity check
    {
        app_stateMachine_setNextState(app_inverterOnState_get());
    }
}

static void initStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();

    app_pumpControl_setFlowRate(
        app_canRx_Debug_SetCoolantPump_CustomEnable_get() ? app_canRx_Debug_SetCoolantPump_CustomVal_get() : 600.0f);
}

static void initStateRunOnExit(void)
{
    LOG_INFO("init exit");
}

const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = initStateRunOnEntry,
        .run_on_tick_1Hz   = initStateRunOnTick1Hz,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
        .run_on_exit       = initStateRunOnExit,
    };

    return &init_state;
}
