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
static constexpr uint8_t SW_DEBOUNCE_TICKS = 5;

Switch torque_vectoring_sw(SW_DEBOUNCE_TICKS);
Switch launch_control_sw(SW_DEBOUNCE_TICKS);
Switch regen_sw(SW_DEBOUNCE_TICKS);
Switch start_sw(SW_DEBOUNCE_TICKS);
Switch telem_sw(SW_DEBOUNCE_TICKS);
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
static std::array<digit, DIGITS> display;
// ReSharper disable once CppParameterMayBeConstPtrOrRef
std::expected<void, ErrorCode> write(std::array<digit, DIGITS> &data)
{
    display = data;
    return {};
}
} // namespace io::seven_seg

#include "io_canQueues.hpp"
static void                overflow_callback() {}
static void                overflow_callback(uint32_t) {}
io::queue<io::CanMsg, 128> can_tx_queue{ "", overflow_callback, overflow_callback };
io::queue<io::CanMsg, 128> can_rx_queue{ "", overflow_callback, overflow_callback };