// TODO: Reset precharge limit exceeded

#include <cstddef>

extern "C" {
#include "app_states.hpp"
#include "app_canTx.h"
#include "app_timer.h"
}

namespace app::prechargeLatchState
{

constexpr uint32_t PRECHARGE_LATCH_TIMEOUT_MS = 3000U; // 3 seconds
static TimerChannel precharge_latch_timer;

static void runOnEntry()
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_LATCH_STATE);

    app_timer_init(&precharge_latch_timer, PRECHARGE_LATCH_TIMEOUT_MS);
    app_timer_restart(&precharge_latch_timer);
}

static void runOnTick100Hz()
{
    if (app_timer_updateAndGetState(&precharge_latch_timer) == TIMER_STATE_EXPIRED)
    {
        app::StateMachine::set_next_state(&init_state);
    }
}

static void runOnExit()
{
    // Nothing to do here yet
}

} // namespace app::prechargeLatchState

extern "C" {
const State precharge_latch_state = {
    .name              = "PRECHARGE LATCH",
    .run_on_entry      = app::prechargeLatchState::runOnEntry,
    .run_on_tick_100Hz = app::prechargeLatchState::runOnTick100Hz,
    .run_on_exit       = app::prechargeLatchState::runOnExit,
};
}