#include "app_states.h"
#include "app_canTx.h"

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

static void driveStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_DRIVE_STATE);
    app_powerManager_updateConfig(power_manager_state);
}

static void driveStateRunOnTick1Hz(void) {}

static void driveStateRunOnTick100Hz(void) {}

static void driveStateRunOnExit(void) {}

State drive_state = {
    .name              = "DRIVE",
    .run_on_entry      = driveStateRunOnEntry,
    .run_on_tick_1Hz   = driveStateRunOnTick1Hz,
    .run_on_tick_100Hz = driveStateRunOnTick100Hz,
    .run_on_exit       = driveStateRunOnExit,
};
