#include "test_fakes.hpp"
#include "io_tractiveSystem.hpp"
#include "io_charger.hpp"

namespace io::charger
{
    static app::can_utils::ChargerConnectedType connection_status = 
        app::can_utils::ChargerConnectedType::CHARGER_DISCONNECTED;
    static float evse_duty_cycle = 0.0f;
    static float evse_frequency = 0.0f;

    app::can_utils::ChargerConnectedType getConnectionStatus() { return connection_status; }
    float getCPDutyCycle() { return evse_duty_cycle; }
    float getCPFrequency() { return evse_frequency; }
} // namespace io::charger

namespace io::ts 
{
    static float voltage               = 0.0f;
    static float current_low_res       = 0.0f;
    static float current_high_res      = 0.0f;
    static bool voltage_sns_diag_state = true;
    static bool current_sns_diag_state = true;

    float getVoltage() { return voltage; }
    float getCurrentLowResolution() { return current_low_res; }
    float getCurrentHighResolution() { return current_high_res; }
    bool getVoltageSnsDiagState() { return voltage_sns_diag_state; }
    bool getCurrentSnsDiagState() { return current_sns_diag_state; }
} // namespace io::ts

namespace fakes::ts 
{
    void setVoltage(float voltage)              { io::ts::voltage = voltage; }
    void setCurrentLowResolution(float amps)    { io::ts::current_low_res = amps; }
    void setCurrentHighResolution(float amps)   { io::ts::current_high_res = amps; }
    void setVoltageSnsDiagState(bool ok)        { io::ts::voltage_sns_diag_state = ok; }
    void setCurrentSnsDiagState(bool ok)        { io::ts::current_sns_diag_state = ok; }

    void reset() 
    {
        io::ts::voltage = io::ts::current_low_res = io::ts::current_high_res = 0.0f;
        io::ts::voltage_sns_diag_state = io::ts::current_sns_diag_state = true;
    }
} // namespace fakes::ts

namespace fakes::charger
{
    void setConnectionStatus(app::can_utils::ChargerConnectedType status){ io::charger::connection_status = status; }
    void setCPDutyCycle(float duty_cycle) { io::charger::evse_duty_cycle = duty_cycle; }
    void setCPFrequency(float frequency) { io::charger::evse_frequency = frequency; }
} // namespace fakes::charger