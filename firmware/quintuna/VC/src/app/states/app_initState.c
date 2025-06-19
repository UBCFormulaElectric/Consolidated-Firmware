#include "app_states.h"
#include "app_powerManager.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include <stdbool.h>

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = false, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = false, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_L_RAD]   = { .efuse_enable = false, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = false, .timeout = 200, .max_retry = 5 } }
};

static void initStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_INIT_STATE);
    app_powerManager_updateConfig(power_manager_state);
}
static void initStateRunOnTick100Hz(void)
{
    const ContactorState air_minus_closed = app_canRx_BMS_IrNegative_get();
    if (air_minus_closed == CONTACTOR_STATE_CLOSED)
    {
        app_stateMachine_setNextState(&inverterOn_state);
    }
}
static void initStateRunOnExit(void) {}

State init_state = { .name              = "INIT",
                     .run_on_entry      = initStateRunOnEntry,
                     .run_on_tick_100Hz = initStateRunOnTick100Hz,
                     .run_on_exit       = initStateRunOnExit };
