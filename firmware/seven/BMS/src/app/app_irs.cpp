#include "io_irs.hpp"
#include "io_irs.cpp"
#include "app_irs.hpp"
#include "app_timer.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"

//Strictly externally linked for unit testing purposes
static constexpr uint8_t N_DEBOUNCE_PERIOD_MS = 200;

static app::Timer negative_opened_debounce_timer{ N_DEBOUNCE_PERIOD_MS };

namespace app::irs
{
// bool negativeOpenedDebounced()
// {
//     const bool negative_opened = io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;
//     return negative_opened_debounce_timer.runIfCondition(negative_opened) == app::Timer::TimerState::EXPIRED;
// }

// restart the debounce timer on each opening
static bool was_negative_open       = false;
static bool negative_open_debounced = false;

void updateNegativeOpenDebounce()
{
    const bool is_open = io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;

    // Start a fresh timer on the first open reading of each opening
    if (is_open && !was_negative_open)
    {
        negative_opened_debounce_timer.restart();
    }

    // A reading other than open resets the timer for the next opening
    negative_open_debounced =
        negative_opened_debounce_timer.runIfCondition(is_open) == app::Timer::TimerState::EXPIRED;

    was_negative_open = is_open;
}

bool negativeOpenedDebounced()
{
    return negative_open_debounced;
}

// void negativeOpenedDebounceTimerRestart()
// {
//      negative_opened_debounce_timer.restart();
//  }

void broadcast()
{
    const app::can_utils::ContactorState negative_closed  = io::irs::negativeState();
    const app::can_utils::ContactorState positive_closed  = io::irs::positiveState();
    const app::can_utils::ContactorState precharge_closed = io::irs::prechargeState();

    app::can_tx::BMS_IrNegative_set(negative_closed);
    app::can_tx::BMS_IrPositive_set(positive_closed);
    app::can_tx::BMS_PrechargeRelay_set(precharge_closed);
}
} // namespace app::irs