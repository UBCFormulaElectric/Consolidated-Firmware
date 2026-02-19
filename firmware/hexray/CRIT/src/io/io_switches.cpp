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
bool telem_mark_get()
{
    return telem_sig.readPin();
}
} // namespace io::switches