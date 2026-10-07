#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_switches.hpp"
#include "io_switches.hpp"
#include "app_signal.hpp"

using namespace app::can_utils;

namespace app::switches
{
static constexpr uint32_t DEBOUNCE_TIME = 10U;
namespace
{
    bool getDebouncedstate(const bool raw, Signal &signal)
    {
        const Signal::SignalState state = signal.get_updated_state(raw, !raw);
        return state == Signal::SignalState::ACTIVE;
    }
} // namespace

 
bool start_get()
{
    static Signal start_signal(DEBOUNCE_TIME, DEBOUNCE_TIME);
    return getDebouncedstate(io::switches::start_get(), start_signal);
}

bool telem_get()
{
    static Signal telem_signal(DEBOUNCE_TIME, DEBOUNCE_TIME);
    return getDebouncedstate(io::switches::telem_mark_get(), telem_signal);
}

void broadcast()
{
    // update the state from the switches
    app::can_tx::CRIT_StartButton_set(static_cast<SwitchState>(start_get()));
}
} // namespace app::switches