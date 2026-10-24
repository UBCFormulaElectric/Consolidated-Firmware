#include <cstdint>

#include "app_charger.hpp"
#include "io_charger.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

namespace app::charger
{

float getOutputCurrent()
{
    return app::can_rx::Elcon_OutputCurrent_get();
}

// Stores current charger status in struct for easy access
ElconFaultStatus getFaultStatus()
{
    ElconFaultStatus rx{ .hardware_failure     = app::can_rx::Elcon_HardwareFailure_get(),
                         .over_temperature     = app::can_rx::Elcon_ChargerOverTemperature_get(),
                         .input_voltage_fault  = app::can_rx::Elcon_InputVoltageError_get(),
                         .charging_disabled    = app::can_rx::Elcon_ChargingDisabled_get(), 
                         .comm_timeout         = app::can_rx::Elcon_CommunicationTimeout_get() 
    };
    return rx;
}

// Sends a charging command to the BMS CAN TX table
void setChargingConfig(const ElconChargingConfig &cmd) 
{
    app::can_tx::BMS_MaxChargingVoltage_set(cmd.max_voltage_v);
    app::can_tx::BMS_MaxChargingCurrent_set(cmd.max_current_a);
    app::can_tx::BMS_StopCharging_set(cmd.stop_charging);
}   

float getAvailableCurrent()
{
    const float evse_duty_cycle = io::charger::getCPDutyCycle();

    if (evse_duty_cycle >= 10.0f && evse_duty_cycle <= 85.0f) 
    {
        return evse_duty_cycle * 0.6f;
    } 
    else if (evse_duty_cycle > 85.0f && evse_duty_cycle <= 96.0f)
    {
        return (evse_duty_cycle - 64.0f) * 2.5f;
    }
    return 0.0f;     // Sentinel value for invalid conditions such as duty cycle > 96% (critical error) or < 10% (charger not ready)
}

// Broadcast EVSE measurements and current capability to the BMS CAN TX table
void broadcastEvseStatus()
{
    const float availableCurrent  = getAvailableCurrent();
    const float evseDutyCycle     = io::charger::getCPDutyCycle();
    const float evseFrequency     = io::charger::getCPFrequency();
    app::can_tx::BMS_EvseDutyCycle_set(evseDutyCycle);
    app::can_tx::BMS_EvseFrequency_set(evseFrequency);
    app::can_tx::BMS_EvseAvailableCurrent_set(availableCurrent);
}
} // namespace app::charger
