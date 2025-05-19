#include "app_stateMachine.h"
#include "app_states.h"
#include "app_powerManager.h"
#include "app_timer.h"
#include "io_loadswitches.h"
#include <app_canTx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <stdint.h>

static PowerManagerConfig power_manager_state = {
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

static bool power_sequencing_done = false;
static bool ready_for_drive       = false;

static void hvInitStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_HV_INIT_STATE);
    app_powerManager_updateConfig(power_manager_state);
}
static void hvInitStateRunOnTick1Hz(void) {}
static void hvInitStateRunOnTick100Hz(void) {}
static void hvInitStateRunOnExit(void) {}

State hvInit_state = { .name              = "HV INIT",
                       .run_on_entry      = hvInitStateRunOnEntry,
                       .run_on_tick_1Hz   = hvInitStateRunOnTick1Hz,
                       .run_on_tick_100Hz = hvInitStateRunOnTick100Hz,
                       .run_on_exit       = hvInitStateRunOnExit };
