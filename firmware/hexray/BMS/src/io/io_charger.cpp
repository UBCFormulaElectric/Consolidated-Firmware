#include "io_charger.hpp"
#include "hw_gpios.hpp"
#include "hw_pwms.hpp"

namespace io::charger
{

ChargerConnectedType getConnectionStatus()
{
    constexpr int kMinHz = 990;
    constexpr int kMaxHz = 1010;

    const int freq = hw_pwmInput_getFrequency(&evse_pwm_input);
    if (kMinHz <= freq && freq <= kMaxHz)
        return CHARGER_CONNECTED_EVSE;

    if (hw_gpio_readPin(&n_evse_i_lim_pin))
        return CHARGER_CONNECTED_WALL;

    return CHARGER_DISCONNECTED;
}

float getDutyCycle()
{
    return hw::pwmInput::getDutyCycle(&evse_pwm_input);
}

} // namespace io::charger