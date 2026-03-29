#include "io_irs.hpp"
#include "app_irs.hpp"
#include "app_timer.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"

static app::Timer negative_opened_debounce_timer{ app::irs::N_DEBOUNCE_PERIOD_MS };

namespace app::irs
{
bool negativeOpenedDebounced()
{
    bool negative_opened = io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;
    return negative_opened_debounce_timer.runIfCondition(negative_opened) == app::Timer::TimerState::EXPIRED;
}

void negativeOpenedDebounceTimerRestart()
{
    negative_opened_debounce_timer.restart();
}

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