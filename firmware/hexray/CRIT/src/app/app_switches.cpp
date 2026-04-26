#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_switches.hpp"
#include "io_switches.hpp"

using namespace app::can_utils;

namespace app::switches
{
void broadcast(void)
{
    // update the state from the switches
    app::can_tx::CRIT_TorqueVectoringSwitch_set(static_cast<SwitchState>(io::switches::torque_vectoring_sw.isClosed()));
    app::can_tx::CRIT_LaunchControlSwitch_set(static_cast<SwitchState>(io::switches::launch_control_sw.isClosed()));
    app::can_tx::CRIT_RegenSwitch_set(static_cast<SwitchState>(io::switches::regen_sw.isClosed()));
    app::can_tx::CRIT_StartButton_set(static_cast<SwitchState>(io::switches::start_sw.isClosed()));
}
} // namespace app::switches
