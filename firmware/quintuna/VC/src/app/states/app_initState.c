#include "app_states.h"
#include "app_loadswitches.h"
#include "app_powerManager.h"
#include <app_canTx.h>
#include <app_canUtils.h>
#include <stdbool.h>

static const PowerState power_manager_shutdown_init = {
    .efuses = { [EFUSE_CHANNEL_F_INV]   = false,
                [EFUSE_CHANNEL_RSM]     = true,
                [EFUSE_CHANNEL_BMS]     = true,
                [EFUSE_CHANNEL_R_INV]   = false,
                [EFUSE_CHANNEL_DAM]     = true,
                [EFUSE_CHANNEL_FRONT]   = true,
                [EFUSE_CHANNEL_RL_PUMP] = false,
                [EFUSE_CHANNEL_RR_PUMP] = false,
                [EFUSE_CHANNEL_F_PUMP]  = false,
                [EFUSE_CHANNEL_L_RAD]   = false,
                [EFUSE_CHANNEL_R_RAD]   = false },
};

static void initStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_INIT_STATE);
    app_powerManager_updateConfig(power_manager_shutdown_init);
}
static void initStateRunOnTick1Hz(void) {}
static void initStateRunOnTick100Hz(void) {}
static void initStateRunOnExit(void) {}

State init_state = { .name              = "INIT",
                     .run_on_entry      = initStateRunOnEntry,
                     .run_on_tick_1Hz   = initStateRunOnTick1Hz,
                     .run_on_tick_100Hz = initStateRunOnTick100Hz,
                     .run_on_exit       = initStateRunOnExit };
