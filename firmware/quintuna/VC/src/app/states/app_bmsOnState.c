#include "app_stateMachine.h"
#include "app_states.h"
#include "app_loadswitches.h"
#include "app_powerManager.h"
#include "app_faultHandling.h"
#include <app_canTx.h>
#include <app_canRx.h>
#include <app_canUtils.h>
#include <stdbool.h>

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_RR_PUMP] = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_F_PUMP]  = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = false, .timeout = 200, .max_retry = 5 } }
};

static void bmsOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_BMS_ON_STATE);
    app_powerManager_updateConfig(power_manager_state);
}
static void bmsOnStateRunOnTick100Hz(void)
{
    // Once we have succesfully transitioned here, the BMS will read the state of the VC based on the associated CAN
    // message and then transition to the appropriate stage Note that if the BMS transitons to drive state we transition
    // to PCM_ON state

    // Note: Still unsure what is happening with PCM state as there PCM is still in the air in terms of progress
    // Update: Currently we are having a state in which we tell the PCM MCU to send the turn on command and wait for hv
    // bus to go online

    if (app_canRx_BMS_State_get() == BMS_DRIVE_STATE)
    {
        app_stateMachine_setNextState(&pcmOn_state);
    }
}
static void bmsOnStateRunOnExit(void) {}

State bmsOn_state = { .name              = "BMS ON",
                      .run_on_entry      = bmsOnStateRunOnEntry,
                      .run_on_tick_100Hz = bmsOnStateRunOnTick100Hz,
                      .run_on_exit       = bmsOnStateRunOnExit };
