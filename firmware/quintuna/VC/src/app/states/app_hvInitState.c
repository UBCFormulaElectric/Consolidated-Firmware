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
                [EFUSE_CHANNEL_RL_PUMP] = false,
                [EFUSE_CHANNEL_RR_PUMP] = false,
                [EFUSE_CHANNEL_F_PUMP]  = false,
                [EFUSE_CHANNEL_L_RAD]   = false,
                [EFUSE_CHANNEL_R_RAD]   = false },
};

static bool power_sequencing_done = false;
static bool ready_for_drive       = false;

static void hvInitStateRunOnEntry(void)
{
    app_powerManager_updateConfig(power_manager_hvInit);
    app_canTx_VC_State_set(VC_HV_STATE);
    app_powerSequencing_init();
}
static void hvInitStateRunOnTick1Hz(void) {}
static void hvInitStateRunOnTick100Hz(void)
{
    power_sequencing_done = app_powerSequencing_run();

    if (power_sequencing_done)
    {
        ready_for_drive = app_powerSequencing_checkAllLoads(power_manager_hvInit);

        if (ready_for_drive)
        {
            app_stateMachine_setNextState(&hv_state);
        }
    }
}
static void hvInitStateRunOnExit(void) {}

State hvInit_state = { .name              = "HV INIT",
                       .run_on_entry      = hvInitStateRunOnEntry,
                       .run_on_tick_1Hz   = hvInitStateRunOnTick1Hz,
                       .run_on_tick_100Hz = hvInitStateRunOnTick100Hz,
                       .run_on_exit       = hvInitStateRunOnExit };
