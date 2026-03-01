#include "io_charger.hpp"
#include "hw_gpios.hpp"
#include "hw_pwms.hpp"
#include "app_canUtils.hpp"

using namespace hw::pwm;

namespace io::charger
{

app::can_utils::ChargerConnectedType getConnectionStatus()
{
    constexpr int kMinHz = 990;
    constexpr int kMaxHz = 1010;

    const int freq = static_cast<int>(hw::pwm::evse_pwm_input.get_frequency());
    if (kMinHz <= freq && freq <= kMaxHz)
        return app::can_utils::ChargerConnectedType::CHARGER_CONNECTED_EVSE;

    if (!n_evse_i_lim.readPin())
        return app::can_utils::ChargerConnectedType::CHARGER_CONNECTED_WALL;

    return app::can_utils::ChargerConnectedType::CHARGER_DISCONNECTED;
}

float getDutyCycle()
{
    return hw::pwm::evse_pwm_input.get_dutyCycle();
}

} // namespace io::charger