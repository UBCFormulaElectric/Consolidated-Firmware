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
result<void> update(const config &c)
{
    UNUSED(c);
    return {};
}
result<void> setBrightness(const float brightness)
{
    UNUSED(brightness);
    return {};
}
} // namespace io::leds

#include "io_sevenSeg.hpp"
namespace io::seven_seg
{
result<void> setBrightness(const float brightness)
{
    UNUSED(brightness);
    return {};
}
static std::array<digit, DIGITS> display;
// ReSharper disable once CppParameterMayBeConstPtrOrRef
result<void> write(std::array<digit, DIGITS> &data)
{
    display = data;
    return {};
}
} // namespace io::seven_seg

#include "io_canQueues.hpp"
io::queue<io::CanMsg, 128> can_tx_queue{ "" };
io::queue<io::CanMsg, 128> can_rx_queue{ "" };

#include "io_powerGauge.hpp"

namespace io::power_gauge
{
result<void> update(std::span<led_frame, LEDS> data)
{
    return {};
}
} // namespace io::power_gauge
