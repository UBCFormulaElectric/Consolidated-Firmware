#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_switches.hpp"
#include "io_switches.hpp"
#include "io_leds.hpp"

using namespace app::can_utils;

namespace app::switches
{
static constexpr uint32_t DEBOUNCE_TIME = 20U;

static Signal tv_signal(DEBOUNCE_TIME, DEBOUNCE_TIME);
static Signal launch_signal(DEBOUNCE_TIME, DEBOUNCE_TIME);
static Signal regen_signal(DEBOUNCE_TIME, DEBOUNCE_TIME);
static Signal start_signal(DEBOUNCE_TIME, DEBOUNCE_TIME);

bool getDebouncedstate(const bool raw, Signal &signal)
{
    const Signal::SignalState state = signal.get_updated_state(raw, !raw);
    return (state == Signal::SignalState::ACTIVE) ? true : false;
}

void broadcast(void)
{
    // TODo: enable leds 
    const bool tv_switch    = getDebouncedstate(io::switches::torque_vectoring_get(), tv_signal);
    const bool launch_switch = getDebouncedstate(io::switches::launch_control_get(), launch_signal);
    const bool regen_switch = getDebouncedstate(io::switches::regen_get(), regen_signal);
    const bool start_button = getDebouncedstate(io::switches::start_get(), start_signal);

    // update the state from the switches
    app::can_tx::CRIT_TorqueVectoringSwitch_set(static_cast<SwitchState>(tv_switch));
    app::can_tx::CRIT_LaunchControlSwitch_set(static_cast<SwitchState>(launch_switch));
    app::can_tx::CRIT_RegenSwitch_set(static_cast<SwitchState>(regen_switch));
    app::can_tx::CRIT_StartButton_set(static_cast<SwitchState>(start_button));    
}
} // namespace app::switches
