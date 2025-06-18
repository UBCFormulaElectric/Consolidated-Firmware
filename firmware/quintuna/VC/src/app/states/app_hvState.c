#include "app_stateMachine.h"
#include "app_states.h"
#include "app_powerManager.h"
#include "app_warningHandling.h"
#include "io_loadswitches.h"
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
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_L_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 } }
};

static bool prev_start_switch_pos;

static void hvStateRunOnEntry(void)
{
    app_powerManager_updateConfig(power_manager_state);
    app_canTx_VC_State_set(VC_HV_ON_STATE);
    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    prev_start_switch_pos = true;
}

static void hvStateRunOnTick100Hz(void)
{
    const bool curr_start_switch_on     = app_canRx_CRIT_StartSwitch_get();
    const bool was_start_switch_enabled = !prev_start_switch_pos && curr_start_switch_on;
    const bool is_brake_actuated        = app_canRx_FSM_BrakeActuated_get();
    const bool inverters_warning        = app_warningHandling_inverterStatus();
    if (is_brake_actuated && was_start_switch_enabled)
    {
        // Transition to drive state when start-up conditions are passed (see
        // EV.10.4.3):
        app_stateMachine_setNextState(&drive_state);
    }
    else if (inverters_warning)
    {
        app_stateMachine_setNextState(&hvInit_state);
        app_canTx_VC_Info_InverterRetry_set(true);
    }

    prev_start_switch_pos = curr_start_switch_on;
}
static void hvStateRunOnExit(void) {}

State hv_state = { .name              = "HV",
                   .run_on_entry      = hvStateRunOnEntry,
                   .run_on_tick_100Hz = hvStateRunOnTick100Hz,
                   .run_on_exit       = hvStateRunOnExit };