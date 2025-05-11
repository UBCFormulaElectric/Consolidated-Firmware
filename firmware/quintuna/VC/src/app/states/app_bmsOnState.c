#include "app_states.h"
#include "app_loadswitches.h"
#include "app_powerManager.h"
#include <app_canTx.h>
#include <app_canUtils.h>
#include <stdbool.h>

static const PowerState power_manager_shutdown_bms_on = {
    .efuses = {
    [EFUSE_CHANNEL_F_INV] = true,
    [EFUSE_CHANNEL_RSM] = true,
    [EFUSE_CHANNEL_BMS] = true,
    [EFUSE_CHANNEL_R_INV] = true,
    [EFUSE_CHANNEL_DAM] = true,
    [EFUSE_CHANNEL_FRONT] = true,
    [EFUSE_CHANNEL_RL_PUMP] = false,
    [EFUSE_CHANNEL_RR_PUMP] = false,
    [EFUSE_CHANNEL_F_PUMP] = false,
    [EFUSE_CHANNEL_L_RAD] = false,
    [EFUSE_CHANNEL_R_RAD] = false
    },
};


static void bmsOnStateRunOnEntry(void) {
    app_canTx_VC_State_set(VC_BMS_ON_STATE);
    app_powerManager_updateConfig(power_manager_shutdown_bms_on);
}
static void bmsOnStateRunOnTick1Hz(void) {}
static void bmsOnStateRunOnTick100Hz(void) {}
static void bmsOnStateRunOnExit(void) {}

State bmsOn_state = { .name              = "BMS ON",
                      .run_on_entry      = bmsOnStateRunOnEntry,
                      .run_on_tick_1Hz   = bmsOnStateRunOnTick1Hz,
                      .run_on_tick_100Hz = bmsOnStateRunOnTick100Hz,
                      .run_on_exit       = bmsOnStateRunOnExit };
