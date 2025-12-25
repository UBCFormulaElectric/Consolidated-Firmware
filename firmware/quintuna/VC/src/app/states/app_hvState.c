#include "app_stateMachine.h"
#include "app_states.h"
#include "app_powerManager.h"
#include "app_startSwitch.h"
#include "app_inverter.h"
#include "io_loadswitches.h"
#include <app_canAlerts.h>
#include <app_canTx.h>
#include <app_canRx.h>
#include <app_canUtils.h>
#include <stdbool.h>

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = false, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 } }
};

static void hvStateRunOnEntry(void)
{
    app_powerManager_updateConfig(power_manager_state);
    app_canTx_VC_State_set(VC_HV_ON_STATE);
    app_canAlerts_VC_Info_InverterRetry_set(false);
}

static void hvStateRunOnTick100Hz(void)
{
    // Conditions for entering drive state: minimum 50% braking and start switch
    // TODO: change this to a faster method after fault recovery
    const bool is_brake_actuated = app_canRx_FSM_BrakeActuated_get();
    if (is_brake_actuated && app_startSwitch_hasRisingEdge())
    {
        // Transition to drive state when start-up conditions are passed (see
        // EV.10.4.3):
        app_stateMachine_setNextState(&drive_state);
    }
}
static void hvStateRunOnExit(void) {}

State hv_state = { .name              = "HV",
                   .run_on_entry      = hvStateRunOnEntry,
                   .run_on_tick_100Hz = hvStateRunOnTick100Hz,
                   .run_on_exit       = hvStateRunOnExit };
