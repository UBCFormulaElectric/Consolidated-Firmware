#include "app_initState.h"
#include "app_allStates.h"
#include "app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

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
};

static void initStateRunOnEntry(void)
{
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
    io_efuse_setChannel(EFUSE_CHANNEL_BUZZER, false);
}

static void initStateRunOnTick100Hz(void)
{
    // Turn on inverters when requested to do so by the BMS (so we can power them up before HV is applied).
    // Also, turn them on if HV is already up/coming up.
    const bool enable_inverters = app_canRx_BMS_State_get() == BMS_INVERTER_ON_STATE ||
                                  app_canRx_BMS_State_get() == BMS_PRECHARGE_STATE ||
                                  app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    bool efuse_fault = app_powerManager_checkEfuses(POWER_MANAGER_SHUTDOWN);
    if (enable_inverters && !efuse_fault)
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

const State *app_initState_get(void)
{
    static State init_state = {
        .name              = "INIT",
        .run_on_entry      = initStateRunOnEntry,
        .run_on_tick_1Hz   = initStateRunOnTick1Hz,
        .run_on_tick_100Hz = initStateRunOnTick100Hz,
    };

    return &init_state;
}
