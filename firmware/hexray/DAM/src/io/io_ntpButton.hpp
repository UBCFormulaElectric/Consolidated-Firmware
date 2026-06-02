#pragma once

#ifndef TARGET_TEST
#include "hw_gpios.hpp"
#endif

namespace io::ntpButton
{
#ifdef TARGET_TEST
[[nodiscard]] bool isPressed();
#else
[[nodiscard]] inline bool isPressed()
{
    return ntp_pin.readPin();
}
#endif
} // namespace io::ntpButton
