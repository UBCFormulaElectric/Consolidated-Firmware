#include "io_switches.hpp"
#include "hw_gpios.hpp"

namespace io::switches
{
bool start_get()
{
    return not push_drive_sig.readPin();
}

bool telem_mark_get()
{
    return not telem_sig.readPin();
}

bool torque_vectoring_get()
{
    return not torque_vectoring.readPin();
}

bool regen_get()
{
    return not regen.readPin();
}

} // namespace io::switches
