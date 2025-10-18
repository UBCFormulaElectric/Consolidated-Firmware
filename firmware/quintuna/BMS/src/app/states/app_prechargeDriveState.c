#include "states/app_states.h"

#include "app_precharge.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_timer.h"
#include "io_irs.h"
#include "io_log.h"

#include <assert.h>

static void app_prechargeDriveStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_DRIVE_STATE);

    app_precharge_init();
    app_precharge_restart();
}

static void app_prechargeDriveStateRunOnTick100Hz(void)
{
    switch (app_precharge_poll(false))
    {
        case PRECHARGE_STATE_COOLDOWN:
            io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
            break;
        case PRECHARGE_STATE_RUNNING:
            io_irs_setPrecharge(CONTACTOR_STATE_CLOSED);
            break;
        case PRECHARGE_STATE_FAILED_CRITICAL: // precharge failed multiple times
            io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
            app_stateMachine_setNextState(&precharge_latch_state);
            break;
        case PRECHARGE_STATE_FAILED:
            io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
            LOG_ERROR("precharge failed, retrying");
            break;
        case PRECHARGE_STATE_SUCCESS:
            io_irs_setPositive(CONTACTOR_STATE_CLOSED);
            break;
        default:
            assert(0);
            break;
    }
}

static void app_prechargeDriveStateRunOnExit(void)
{
    io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
}

const State precharge_drive_state = {
    .name              = "PRECHARGE DRIVE",
    .run_on_entry      = app_prechargeDriveStateRunOnEntry,
    .run_on_tick_100Hz = app_prechargeDriveStateRunOnTick100Hz,
    .run_on_exit       = app_prechargeDriveStateRunOnExit,
};
