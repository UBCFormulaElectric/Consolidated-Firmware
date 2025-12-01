// app_prechargeChargeState.cpp

#include <cassert>

#include "app_states.hpp"
#include "app_precharge.hpp"
#include "io_irs.hpp"
#include "io_log.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
}

namespace app::states::prechargeChargeState
{

static void runOnEntry()
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_CHARGE_STATE);

    app::precharge::init();
    app::precharge::restart();
}

static void runOnTick100Hz()
{
    switch (app::precharge::poll(false))
    {
        case app::precharge::State::RUNNING:
            io::irs::setPrecharge(CONTACTOR_STATE_OPEN);
            break;

        case app::precharge::State::COOLDOWN:
            io::irs::setPrecharge(CONTACTOR_STATE_CLOSED);
            break;

        case app::precharge::State::FAILED_CRITICAL:
            // Precharge failed multiple times, abort charging
            io::irs::setPrecharge(CONTACTOR_STATE_OPEN);

            // Prevent unintended re-entry into charge state
            app_canRx_Debug_StartCharging_update(false);

            app::StateMachine::set_next_state(&precharge_latch_state);
            break;

        case app::precharge::State::FAILED:
            io::irs::setPrecharge(CONTACTOR_STATE_OPEN);
            LOG_ERROR("Precharge failed, retrying");
            break;

        case app::precharge::State::SUCCESS:
            // Precharge succeeded → close AIR+ and move to charge init
            io::irs::setPositive(CONTACTOR_STATE_CLOSED);
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
    io::irs::setPrecharge(CONTACTOR_STATE_OPEN);
}

} // namespace app::states::prechargeChargeState

const app::State precharge_charge_state = {
    .name              = "PRECHARGE CHARGE",
    .run_on_entry      = app::states::prechargeChargeState::runOnEntry,
    .run_on_tick_100Hz = app::states::prechargeChargeState::runOnTick100Hz,
    .run_on_exit       = app::states::prechargeChargeState::runOnExit,
};