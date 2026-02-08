#include <cassert>
#include "app_precharge.hpp"
#include "io_irs.hpp"
#include "app_stateMachine.hpp"
#include "states/app_states.hpp"
#include "io_log.hpp"
#include "app_stateMachine.hpp"

extern "C"
{
#include "app_canTx.h"
}

namespace app::states::perchargeDriveState
{
static void prechargeDriveStateRunOnEntry()
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_DRIVE_STATE);
    app::precharge::init();
    app::precharge::restart();
}

static void prechargeDriveStateRunOnTick100Hz()
{
    switch (app::precharge::poll(false))
    {
        case app::precharge::State::COOLDOWN:
            io::irs::setPrecharge(CONTACTOR_STATE_OPEN);
            break;
        case app::precharge::State::RUNNING:
            io::irs::setPrecharge(CONTACTOR_STATE_CLOSED);
            break;
        case app::precharge::State::FAILED_CRITICAL: // precharge failed multiple times
            io::irs::setPrecharge(CONTACTOR_STATE_OPEN);
            app::StateMachine::set_next_state(&precharge_latch_state);
            break;
        case app::precharge::State::FAILED:
            io::irs::setPrecharge(CONTACTOR_STATE_OPEN);
            LOG_ERROR("precharge failed, retrying");
            break;
        case app::precharge::State::SUCCESS:
            io::irs::setPositive(CONTACTOR_STATE_CLOSED);
            app::StateMachine::set_next_state(&drive_state);
            break;
        default:
            assert(0);
            break;
    }
}

static void prechargeDriveStateRunOnExit()
{
    io::irs::setPrecharge(CONTACTOR_STATE_OPEN);
}
} // namespace app::states::perchargeDriveState

const app::State precharge_drive_state = {
    .name              = "PRECHARGE DRIVE",
    .run_on_entry      = app::states::perchargeDriveState::prechargeDriveStateRunOnEntry,
    .run_on_tick_1Hz   = nullptr,
    .run_on_tick_100Hz = app::states::perchargeDriveState::prechargeDriveStateRunOnTick100Hz,
    .run_on_exit       = app::states::perchargeDriveState::prechargeDriveStateRunOnExit,
};