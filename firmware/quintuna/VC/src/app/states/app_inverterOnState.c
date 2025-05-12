#include "app_stateMachine.h"
#include "app_states.h"
#include "app_loadswitches.h"
#include "app_powerManager.h"
#include <app_canTx.h>
#include <app_canUtils.h>
#include <stdbool.h>

static const PowerState power_manager_inverter_on = {
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

static void inverterOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_INVERTER_ON_STATE);
    app_powerManager_updateConfig(power_manager_inverter_on);
}

static void inverterOnStateRunOnTick1Hz(void) {}
static void inverterOnStateRunOnTick100Hz(void) {
    //here we need to check if the inverters are alive and are sending us active can messages
    //TODO: Configure inverter CAN messages

    //Once inverters are online transition to next state which is BMS on state

    //const bool inverters_on = inverter one and two are on
    

    // if (inverters_on) {
    //     app_stateMachine_setNextState(&bmsOn_state);
    // }
}
static void inverterOnStateRunOnExit(void) {}

State inverterOn_state = { .name              = "INVERTER ON",
                           .run_on_entry      = inverterOnStateRunOnEntry,
                           .run_on_tick_1Hz   = inverterOnStateRunOnTick1Hz,
                           .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
                           .run_on_exit       = inverterOnStateRunOnExit };