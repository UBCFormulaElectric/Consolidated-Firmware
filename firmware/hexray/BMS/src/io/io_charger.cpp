#include "io_charger.hpp"
#include "hw_gpios.hpp"
#include "hw_pwms.hpp"

namespace io::charger
{

app::can_utils::ChargerConnectedType getConnectionStatus()
{
    constexpr float kMinHz = 990.0f;
    constexpr float kMaxHz = 1010.0f;

    const float freq = hw::pwms::evse_pwm_input.get_frequency();
    if (kMinHz <= freq && freq <= kMaxHz)
        return app::can_utils::ChargerConnectedType::CHARGER_CONNECTED_EVSE;

    if (n_evse_i_lim.readPin())
        return app::can_utils::ChargerConnectedType::CHARGER_CONNECTED_WALL;

    return app::can_utils::ChargerConnectedType::CHARGER_DISCONNECTED;
}

float getCPDutyCycle()
{
    return hw::pwms::evse_pwm_input.get_dutyCycle();
}

} // namespace io::charger
