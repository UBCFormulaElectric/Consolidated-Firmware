#include "app_stateMachine.h"
#include "app_states.h"
#include "app_powerManager.h"
#include "io_loadswitches.h"
#include <app_canTx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <stdint.h>

static PowerState power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                [EFUSE_CHANNEL_RR_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                [EFUSE_CHANNEL_F_PUMP]  = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                [EFUSE_CHANNEL_L_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 } }
};

static void hvStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_HV_ON_STATE);
    app_powerManager_updateConfig(power_manager_state);
}
static void hvStateRunOnTick1Hz(void) {}
static void hvStateRunOnTick100Hz(void) {}
static void hvStateRunOnExit(void) {}

State hv_state = { .name              = "HV",
                   .run_on_entry      = hvStateRunOnEntry,
                   .run_on_tick_1Hz   = hvStateRunOnTick1Hz,
                   .run_on_tick_100Hz = hvStateRunOnTick100Hz,
                   .run_on_exit       = hvStateRunOnExit };