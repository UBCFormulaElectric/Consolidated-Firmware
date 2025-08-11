#include "app_states.h"
#include "app_powerManager.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canUtils.h"
#include <app_canAlerts.h>
#include <stdbool.h>

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = false, .timeout = 200, .max_retry = 5 } }
};

static void initStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_INIT_STATE);
    app_powerManager_updateConfig(power_manager_state);
    app_canAlerts_VC_Info_InverterRetry_set(false);

    app_canTx_VC_INVFRTorqueSetpoint_set(0);
    app_canTx_VC_INVRRTorqueSetpoint_set(0);
    app_canTx_VC_INVFLTorqueSetpoint_set(0);
    app_canTx_VC_INVRLTorqueSetpoint_set(0);

    app_canTx_VC_INVFLTorqueLimitPositive_set(0);
    app_canTx_VC_INVFRTorqueLimitPositive_set(0);
    app_canTx_VC_INVRLTorqueLimitPositive_set(0);
    app_canTx_VC_INVRRTorqueLimitPositive_set(0);

    app_canTx_VC_INVFLTorqueLimitNegative_set(0);
    app_canTx_VC_INVFRTorqueLimitNegative_set(0);
    app_canTx_VC_INVRLTorqueLimitNegative_set(0);
    app_canTx_VC_INVRRTorqueLimitNegative_set(0);
}
static void initStateRunOnTick100Hz(void)
{
    const ContactorState air_minus_closed = app_canRx_BMS_IrNegative_get();
    if (air_minus_closed == CONTACTOR_STATE_CLOSED)
    {
        app_stateMachine_setNextState(&inverterOn_state);
    }
}
static void initStateRunOnExit(void) {}

State init_state = { .name              = "INIT",
                     .run_on_entry      = initStateRunOnEntry,
                     .run_on_tick_100Hz = initStateRunOnTick100Hz,
                     .run_on_exit       = initStateRunOnExit };
