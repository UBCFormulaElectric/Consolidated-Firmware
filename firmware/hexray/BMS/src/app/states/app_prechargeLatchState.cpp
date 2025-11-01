// TODO: Reset precharge limit exceeded

#include <cstddef>

#include "app_states.hpp"
#include "app_timer.hpp"

extern "C" {
#include "app_canTx.h"
}

namespace app::states::prechargeLatchState
{

constexpr uint32_t PRECHARGE_LATCH_TIMEOUT_MS = 3000U; // 3 seconds
static app::Timer precharge_latch_timer{PRECHARGE_LATCH_TIMEOUT_MS};

static void runOnEntry()
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_LATCH_STATE);
    precharge_latch_timer.restart();
}

static void runOnTick100Hz()
{
    if (precharge_latch_timer.updateAndGetState() == Timer::TimerState::EXPIRED)
    {
        app::StateMachine::set_next_state(&init_state);
    }
}

static void runOnExit()
{
    // Nothing to do here yet
}

} // namespace app::states::prechargeLatchState

const State precharge_latch_state = {
    .name              = "PRECHARGE LATCH",
    .run_on_entry      = app::states::prechargeLatchState::runOnEntry,
    .run_on_tick_100Hz = app::states::prechargeLatchState::runOnTick100Hz,
    .run_on_exit       = app::states::prechargeLatchState::runOnExit,
};