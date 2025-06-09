#include "app_stateMachine.h"
#include "app_states.h"
#include "app_powerManager.h"
#include "io_loadswitches.h"
#include <app_canTx.h>
#include <app_canRx.h>
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

static bool prev_start_switch_pos;

static void hvStateRunOnEntry(void)
{
    app_powerManager_updateConfig(power_manager_state);
    app_canTx_VC_State_set(VC_HV_ON_STATE);
    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    prev_start_switch_pos = true;
}
static void hvStateRunOnTick1Hz(void) {}

static void hvStateRunOnTick100Hz(void)
{
    const bool curr_start_switch_on     = app_canRx_CRIT_StartSwitch_get();
    const bool was_start_switch_enabled = !prev_start_switch_pos && curr_start_switch_on;
    const bool is_brake_actuated        = app_canRx_FSM_BrakeActuated_get();

    const BmsState bms_state = app_canRx_BMS_State_get();

    if ((BMS_INIT_STATE == bms_state) || (BMS_FAULT_STATE == bms_state))
    {
        app_stateMachine_setNextState(&init_state);
    }

    else if (is_brake_actuated && was_start_switch_enabled)
    {
        // Transition to drive state when start-up conditions are passed (see
        // EV.10.4.3):
        app_stateMachine_setNextState(&drive_state);
    }

    prev_start_switch_pos = curr_start_switch_on;
}
static void hvStateRunOnExit(void) {}

State hv_state = { .name              = "HV",
                   .run_on_entry      = hvStateRunOnEntry,
                   .run_on_tick_1Hz   = hvStateRunOnTick1Hz,
                   .run_on_tick_100Hz = hvStateRunOnTick100Hz,
                   .run_on_exit       = hvStateRunOnExit };