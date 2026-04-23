#include "app_canTx.hpp"
#include "app_switches.hpp"
#include "io_switches.hpp"

namespace app::switches
{
    void broadcast(void)
    {
        // update the state from the switches
        app::can_tx::CRIT_TorqueVectoringSwitch_set(io::switches::torque_vectoring_sw.isClosed());
        app::can_tx::CRIT_LaunchControlSwitch_set(io::switches::launch_control_sw.isClosed());
        app::can_tx::CRIT_RegenSwitch_set(io::switches::regen_sw.isClosed());
        app::can_tx::CRIT_StartButton_set(io::switches::start_sw.isClosed());
    }
} // namespace app::switches
