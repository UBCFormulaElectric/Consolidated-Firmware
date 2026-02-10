#include "io_irs.hpp"
#include "app_irs.hpp"
#include "app_timer.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canUtils.h"
}
static constexpr uint8_t IR_N_DEBOUNCE_PERIOD_MS = 200;
static app::Timer        negative_opened_debounce_timer{ IR_N_DEBOUNCE_PERIOD_MS };

namespace app
{
namespace irs
{
    bool negativeOpenedDebounced()
    {
        bool negative_opened = !io::irs::negativeState();
        if (negative_opened)
        {
            negative_opened_debounce_timer.restart();
            return false;
        }
        return negative_opened_debounce_timer.runIfCondition(negative_opened) == app::Timer::TimerState::EXPIRED;
    }

    void negativeOpenedDebounceTimerRestart()
    {
        negative_opened_debounce_timer.restart();
    }

    void broadcast()
    {
        const ContactorState negative_closed  = io::irs::negativeState();
        const ContactorState positive_closed  = io::irs::positiveState();
        const ContactorState precharge_closed = io::irs::prechargeState();

        app_canTx_BMS_IrNegative_set(negative_closed);
        app_canTx_BMS_IrPositive_set(positive_closed);
        app_canTx_BMS_PrechargeRelay_set(precharge_closed);
    }
} // namespace irs
} // namespace app