#include "critMocks.hpp"

#include "io_rotary.hpp"
namespace io::rotary
{
void init() {}
void setClockwiseCallback(void (*)()) {}
void setCounterClockwiseCallback(void (*)()) {}
void setPushCallback(void (*)()) {}
} // namespace io::rotary

#include "io_switches.hpp"
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
bool telem_mark_get()
{
    return false;
}
} // namespace io::switches

#include "io_leds.hpp"
namespace io::leds
{
std::expected<void, ErrorCode> update(const config &c)
{
    UNUSED(c);
    return {};
}
std::expected<void, ErrorCode> setBrightness(const float brightness)
{
    UNUSED(brightness);
    return {};
}
} // namespace io::leds

#include "io_sevenSeg.hpp"
namespace io::seven_seg
{
std::expected<void, ErrorCode> setBrightness(const float brightness)
{
    UNUSED(brightness);
    return {};
}
} // namespace io::seven_seg