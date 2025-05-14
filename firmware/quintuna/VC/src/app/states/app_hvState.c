#include "app_stateMachine.h"
#include "app_states.h"
#include "app_powerManager.h"
#include "io_loadswitches.h"
#include <app_canTx.h>
#include <app_canUtils.h>
#include <stdbool.h>
#include <stdint.h>
#include "app_powerSequencing.h"

static const PowerState power_manager_hvInit = {
    .efuses = { [EFUSE_CHANNEL_F_INV]   = true,
                [EFUSE_CHANNEL_RSM]     = true,
                [EFUSE_CHANNEL_BMS]     = true,
                [EFUSE_CHANNEL_R_INV]   = true,
                [EFUSE_CHANNEL_DAM]     = true,
                [EFUSE_CHANNEL_FRONT]   = true,
                [EFUSE_CHANNEL_RL_PUMP] = true,
                [EFUSE_CHANNEL_RR_PUMP] = true,
                [EFUSE_CHANNEL_F_PUMP]  = true,
                [EFUSE_CHANNEL_L_RAD]   = true,
                [EFUSE_CHANNEL_R_RAD]   = true },
};

static void hvStateRunOnEntry(void) {
    app_powerManager_updateConfig(power_manager_hvInit);
    app_canTx_VC_State_set(VC_HV_STATE);
}
static void hvStateRunOnTick1Hz(void) {}
static void hvStateRunOnTick100Hz(void) {}
static void hvStateRunOnExit(void) {}

State hv_state = { .name              = "HV",
                   .run_on_entry      = hvStateRunOnEntry,
                   .run_on_tick_1Hz   = hvStateRunOnTick1Hz,
                   .run_on_tick_100Hz = hvStateRunOnTick100Hz,
                   .run_on_exit       = hvStateRunOnExit };