#include "test_fakes.hpp"
#include "io_charger.hpp"

namespace io::charger
{
    static app::can_utils::ChargerConnectedType connection_status = 
        app::can_utils::ChargerConnectedType::CHARGER_DISCONNECTED;
    static float evse_duty_cycle = 0.0f;
    static float evse_frequency = 0.0f;

    app::can_utils::ChargerConnectedType getConnectionStatus()
    {
        return connection_status;
    }
    float getCPDutyCycle()
    {
        return evse_duty_cycle;
    }
    float getCPFrequency()
    {
        return evse_frequency;
    }
} // namespace io::charger

namespace fakes::charger
{
    void setConnectionStatus(app::can_utils::ChargerConnectedType status)
    {
        io::charger::connection_status = status;
    }
    void setCPDutyCycle(float duty_cycle)
    {
        io::charger::evse_duty_cycle = duty_cycle;
    }
    void setCPFrequency(float frequency)
    {
        io::charger::evse_frequency = frequency;
    }
} // namespace fakes::charger