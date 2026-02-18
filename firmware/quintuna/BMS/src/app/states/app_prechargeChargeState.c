#include "states/app_states.h"

#include "app_precharge.h"
#include "app_timer.h"
#include "io_irs.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_log.h"

#include <assert.h>

static void app_prechargeChargeStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_CHARGE_STATE);

    app_precharge_init();
    app_precharge_restart();
}

static void app_prechargeChargeStateRunOnTick100Hz(void)
{
    switch (app_precharge_poll(false))
    {
        case PRECHARGE_STATE_RUNNING:
            io_irs_setPrecharge(CONTACTOR_STATE_CLOSED);
            break;
        case PRECHARGE_STATE_COOLDOWN:
            io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
            break;
        case PRECHARGE_STATE_FAILED_CRITICAL: // precharge failed multiple times
            io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
            // Just in case we exited charging not due to CAN (fault, etc.) set the CAN table back to false so we don't
            // unintentionally re-enter charge state.
            app_canRx_Debug_StartCharging_update(false);
            app_stateMachine_setNextState(&precharge_latch_state);
            break;
        case PRECHARGE_STATE_FAILED:
            io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
            LOG_ERROR("precharge failed, retrying");
            break;
        case PRECHARGE_STATE_SUCCESS:
            io_irs_setPositive(CONTACTOR_STATE_CLOSED);
            app_stateMachine_setNextState(&charge_init_state);
            break;
        default:
            assert(0);
            break;
    }

    // TODO go back to init state if the charger is disconnected?
    //    we might have to do this in all charger related states
}

static void app_prechargeChargeStateRunOnExit(void)
{
    io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
}

const State precharge_charge_state = {
    .name              = "PRECHARGE CHARGE",
    .run_on_entry      = app_prechargeChargeStateRunOnEntry,
    .run_on_tick_100Hz = app_prechargeChargeStateRunOnTick100Hz,
    .run_on_exit       = app_prechargeChargeStateRunOnExit,
};
