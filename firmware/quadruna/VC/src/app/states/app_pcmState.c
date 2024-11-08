#include "app_pcmState.h"
#include "app_allStates.h"
#include "app_hvInitState.h"
#include "app_initState.h"

#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"

#include "app_powerManager.h"
#include "app_pumpControl.h"
#include "app_faultCheck.h"

#include "io_pcm.h"

static PowerStateConfig power_manager_pcm = {
    .efuses = {
        [EFUSE_CHANNEL_SHDN] = true,
        [EFUSE_CHANNEL_LV] = true,
        [EFUSE_CHANNEL_PUMP] = false,
        [EFUSE_CHANNEL_AUX] = false,
        [EFUSE_CHANNEL_INV_R] = true,
        [EFUSE_CHANNEL_INV_L] = true,
        [EFUSE_CHANNEL_TELEM] = true,
        [EFUSE_CHANNEL_BUZZER] = false,
    },
};

static void pcmStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_PCM_STATE);
    app_powerManager_updateConfig(power_manager_pcm);

    // Enable PCM if HV up
    const bool bms_in_drive = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    io_pcm_set(bms_in_drive);

    // Disable inverters and apply zero torque upon entering pcm state
    app_canTx_VC_LeftInverterEnable_set(true);
    app_canTx_VC_RightInverterEnable_set(true);
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    app_canTx_VC_LeftInverterTorqueLimit_set(0.0f);
    app_canTx_VC_RightInverterTorqueLimit_set(0.0f);
    app_canTx_VC_LeftInverterDirectionCommand_set(INVERTER_REVERSE_DIRECTION);
    app_canTx_VC_RightInverterDirectionCommand_set(INVERTER_FORWARD_DIRECTION);
}

static void pcmStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void pcmStateRunOnTick100Hz(void)
{
    const bool bms_in_drive = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;

    // TODO: check if pcm is good (PCM > 18V)
    const bool is_pcm_good = true;

    if (!bms_in_drive)
    {
        app_stateMachine_setNextState(app_initState_get());
    }
    else if (is_pcm_good && bms_in_drive)
    {
        io_pcm_set(true);
        app_stateMachine_setNextState(app_hvInitState_get());
    }
}

const State *app_pcmState_get(void)
{
    static State pcm_state = {
        .name              = "PCM",
        .run_on_entry      = pcmStateRunOnEntry,
        .run_on_tick_1Hz   = pcmStateRunOnTick1Hz,
        .run_on_tick_100Hz = pcmStateRunOnTick100Hz,
    };

    return &pcm_state;
}
