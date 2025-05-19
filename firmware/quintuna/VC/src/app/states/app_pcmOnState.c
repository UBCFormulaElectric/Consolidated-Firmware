#include "app_states.h"
#include "app_canTx.h"
#include <app_canUtils.h>


static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = false, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = false, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_RR_PUMP] = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_F_PUMP]  = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_L_RAD]   = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = false, .timeout = 200, .max_retry = 5 } }
};

static void pcmOntateRunOnEntry(void) {
    app_canTx_VC_State_set(VC_PCM_ON_STATE);
    app_powerManager_updateConfig(power_manager_state);
}
static void pcmOnStateRunOnTick1Hz(void) {}
static void pcmOnStateRunOnTick100Hz(void) {}
static void pcmOnStateRunOnExit(void) {}

State pcmOn_state = { .name              = "PCM ON",
                      .run_on_entry      = pcmOntateRunOnEntry,
                      .run_on_tick_1Hz   = pcmOnStateRunOnTick1Hz,
                      .run_on_tick_100Hz = pcmOnStateRunOnTick100Hz,
                      .run_on_exit       = pcmOnStateRunOnExit };