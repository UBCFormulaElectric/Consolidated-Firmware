// app_prechargeChargeState.cpp

#include <cassert>

#include "app_states.hpp"

extern "C" {
#include "app_precharge.h"
#include "io_irs.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_log.h"
}

namespace app::states::prechargeChargeState
{

static void runOnEntry()
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_CHARGE_STATE);

    app_precharge_init();
    app_precharge_restart();
}

static void runOnTick100Hz()
{
    switch (app_precharge_poll(false))
    {
        case PRECHARGE_STATE_RUNNING:
            io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
            break;

        case PRECHARGE_STATE_COOLDOWN:
            io_irs_setPrecharge(CONTACTOR_STATE_CLOSED);
            break;

        case PRECHARGE_STATE_FAILED_CRITICAL:
            // Precharge failed multiple times, abort charging
            io_irs_setPrecharge(CONTACTOR_STATE_OPEN);

            // Prevent unintended re-entry into charge state
            app_canRx_Debug_StartCharging_update(false);

            app::StateMachine::set_next_state(&precharge_latch_state);
            break;

        case PRECHARGE_STATE_FAILED:
            io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
            LOG_ERROR("Precharge failed, retrying");
            break;

        case PRECHARGE_STATE_SUCCESS:
            // Precharge succeeded → close AIR+ and move to charge init
            io_irs_setPositive(CONTACTOR_STATE_CLOSED);
            app::StateMachine::set_next_state(&charge_init_state);
            break;

        default:
            assert(false && "Invalid precharge state");
            break;
    }

    // TODO: Go back to init state if the charger is disconnected?
    // Might need to handle this in all charger-related states.
}

static void runOnExit()
{
    io_irs_setPrecharge(CONTACTOR_STATE_OPEN);
}

} // namespace app::states::prechargeChargeState

const State precharge_charge_state = {
    .name              = "PRECHARGE CHARGE",
    .run_on_entry      = app::states::prechargeChargeState::runOnEntry,
    .run_on_tick_100Hz = app::states::prechargeChargeState::runOnTick100Hz,
    .run_on_exit       = app::states::prechargeChargeState::runOnExit,
};