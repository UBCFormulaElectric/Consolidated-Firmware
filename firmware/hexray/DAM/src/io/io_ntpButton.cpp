#include "io_ntpButton.hpp"
#include "hw_gpios.hpp"

namespace io::ntpButton
{
[[nodiscard]] bool isPressed()
{
    return ntp_pin.readPin();
}
} // namespace io::ntpButton
