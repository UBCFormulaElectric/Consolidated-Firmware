#include "io_ntpButton.hpp"
#include "hw_gpios.hpp"

namespace io::ntpButton
{
bool wasJustPressed()
{
    static bool prev_pressed = false;
    const bool  now_pressed  = ntp_pin.readPin();
    const bool  rising_edge  = now_pressed && !prev_pressed;
    prev_pressed             = now_pressed;
    return rising_edge;
}
} // namespace io::ntpButton