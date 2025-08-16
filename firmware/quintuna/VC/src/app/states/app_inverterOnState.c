#include "app_stateMachine.h"
#include "app_states.h"
#include "app_loadswitches.h"
#include "app_powerManager.h"
#include "app_faultHandling.h"
#include <app_canAlerts.h>
#include <app_canRx.h>
#include <app_canTx.h>
#include <app_canUtils.h>
#include <stdbool.h>

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = false, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = false, .timeout = 200, .max_retry = 5 } }
};

static void inverterOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_INVERTER_ON_STATE);
    app_powerManager_updateConfig(power_manager_state);
    app_canAlerts_VC_Info_InverterRetry_set(false);
}

static void inverterOnStateRunOnTick100Hz(void)
{
    // here we need to check if the inverters are alive and are sending us active can messages

    const bool inverters_bsystemReady =  app_canRx_INVRL_bSystemReady_get() && app_canRx_INVRR_bSystemReady_get();
    if (inverters_bsystemReady)
    {
        app_stateMachine_setNextState(&bmsOn_state);
    }
}
static void inverterOnStateRunOnExit(void) {}

State inverterOn_state = { .name              = "INVERTER ON",
                           .run_on_entry      = inverterOnStateRunOnEntry,
                           .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
                           .run_on_exit       = inverterOnStateRunOnExit };
