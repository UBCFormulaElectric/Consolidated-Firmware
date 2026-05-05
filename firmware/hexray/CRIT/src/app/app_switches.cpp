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

bool torque_vectoring_get()
{
    static Signal tv_signal(DEBOUNCE_TIME, DEBOUNCE_TIME);
    return getDebouncedstate(io::switches::torque_vectoring_get(), tv_signal);
}

bool launch_control_get()
{
    static Signal launch_signal(DEBOUNCE_TIME, DEBOUNCE_TIME);
    return getDebouncedstate(io::switches::launch_control_get(), launch_signal);
}

bool regen_get()
{
    static Signal regen_signal(DEBOUNCE_TIME, DEBOUNCE_TIME);
    return getDebouncedstate(io::switches::regen_get(), regen_signal);
}

bool start_get()
{
    static Signal start_signal(DEBOUNCE_TIME, DEBOUNCE_TIME);
    return getDebouncedstate(io::switches::start_get(), start_signal);
}

void broadcast()
{
    // update the state from the switches
    app::can_tx::CRIT_TorqueVectoringSwitch_set(static_cast<SwitchState>(torque_vectoring_get()));
    app::can_tx::CRIT_LaunchControlSwitch_set(static_cast<SwitchState>(launch_control_get()));
    app::can_tx::CRIT_RegenSwitch_set(static_cast<SwitchState>(regen_get()));
    app::can_tx::CRIT_StartButton_set(static_cast<SwitchState>(start_get()));
}
} // namespace app::switches
