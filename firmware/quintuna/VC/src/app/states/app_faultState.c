#include "app_stateMachine.h"
#include "app_states.h"
#include "app_powerManager.h"

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


static void faultStateRunOnEntry(void) 
{
    app_powerManager_updateConfig(power_manager_state);
}
static void faultStateRunOnTick1Hz(void) {}
static void faultStateRunOnTick100Hz(void) 
{
    
}
static void faultStateRunOnExit(void) {}

State fault_state = { .name              = "FAULT",
                             .run_on_entry      = faultStateRunOnEntry,
                             .run_on_tick_1Hz   = faultStateRunOnTick1Hz,
                             .run_on_tick_100Hz = faultStateRunOnTick100Hz,
                             .run_on_exit       = faultStateRunOnExit };