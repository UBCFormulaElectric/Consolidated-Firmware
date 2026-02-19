#include "critMocks.hpp"

#include "io_rotary.hpp"
#include "io_switches.hpp"

namespace io::rotary
{
void init() {}
void setClockwiseCallback(void (*)()) {}
void setCounterClockwiseCallback(void (*)()) {}
void setPushCallback(void (*)()) {}
} // namespace io::rotary

namespace io::switches
{
bool torque_vectoring_get()
{
    return false;
}
bool launch_control_get()
{
    return false;
}
bool regen_get()
{
    return false;
}
bool start_get()
{
    return false;
}
bool telem_mark_rising_edge_get()
{
    return false;
}
} // namespace io::switches