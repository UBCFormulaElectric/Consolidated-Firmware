#include "app_stateMachine.h"
#include "app_states.h"
#include "app_loadswitches.h"
#include "app_powerManager.h"
#include <app_canTx.h>
#include <app_canRx.h>
#include <app_canUtils.h>
#include <stdbool.h>

static const PowerState power_manager_shutdown_bms_on = {
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

static void bmsOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_BMS_ON_STATE);
    app_powerManager_updateConfig(power_manager_shutdown_bms_on);
}
static void bmsOnStateRunOnTick1Hz(void) {

    //Once we have succesfully transitioned here, the BMS will read the state of the VC based on the associated CAN message and then transition to the appropriate stage
    //Note that if the BMS transitons to drive state we transition to PCM_ON state

    //Note: Still unsure what is happening with PCM state as there PCM is still in the air in terms of progress
    const bool bms_ready_for_drive = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    const bool bms_in_faultstate = app_canRx_BMS_State_get() == BMS_FAULT_STATE;

    if (bms_ready_for_drive){
        app_stateMachine_setNextState(&pcmOn_state);
    }

    else if (bms_in_faultstate) {
        app_stateMachine_setNextState(&fault_state);
    }


}
static void bmsOnStateRunOnTick100Hz(void) {}
static void bmsOnStateRunOnExit(void) {}

State bmsOn_state = { .name              = "BMS ON",
                      .run_on_entry      = bmsOnStateRunOnEntry,
                      .run_on_tick_1Hz   = bmsOnStateRunOnTick1Hz,
                      .run_on_tick_100Hz = bmsOnStateRunOnTick100Hz,
                      .run_on_exit       = bmsOnStateRunOnExit };
