#include "io_switches.hpp"
#include "hw_gpios.hpp"

namespace io::switches
{
bool torque_vectoring_get()
{
    return torque_vectoring_sig.readPin();
}
bool launch_control_get()
{
    return launch_control_sig.readPin();
}
bool regen_get()
{
    return regen_sig.readPin();
}
bool start_get()
{
    return push_drive_sig.readPin();
}
bool telem_mark_rising_edge_get()
{
    static bool last_state    = false;
    const bool  current_state = telem_sig.readPin();
    const bool  rising_edge   = current_state == true and last_state == false;
    last_state                = current_state;
    return rising_edge;
}
} // namespace io::switches