#include "io_ntpButton.hpp"
#include "hw_gpios.hpp"
#include "io_time.hpp"

namespace io::ntpButton
{
bool wasJustPressed()
{
    static bool     prev_pressed = false;
    static uint32_t last_edge_ms = 0;
    static bool     reported     = false;

    constexpr uint32_t kDebounceMs = 50;

    const bool     now_pressed = ntp_pin.readPin();
    const bool     rising_edge = now_pressed && !prev_pressed;
    const bool     any_edge    = now_pressed != prev_pressed;
    const uint32_t now_ms      = io::time::getCurrentMs();

    prev_pressed = now_pressed;

    if (any_edge)
    {
        last_edge_ms = now_ms;
    }

    if (reported && ((now_ms - last_edge_ms) >= kDebounceMs))
    {
        reported = false;
    }

    if (rising_edge && !reported)
    {
        reported = true;
        return true;
    }

    return false;
}
} // namespace io::ntpButton