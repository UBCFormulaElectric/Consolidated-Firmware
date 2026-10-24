#include <cstdint>

#include "app_charger.hpp"
#include "io_charger.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

// EVSE duty cycle thresholds for computing current. Values derived from SAE J1772 standard for EVSE duty cycle
#define EVSE_DUTY_CYCLE_MIN 10.0f
#define EVSE_DUTY_CYCLE_MID 85.0f
#define EVSE_DUTY_CYCLE_MAX 96.0f

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

    if (evse_duty_cycle >= EVSE_DUTY_CYCLE_MIN && evse_duty_cycle <= EVSE_DUTY_CYCLE_MID) 
    {
        return evse_duty_cycle * 0.6f;              // Formula derived from SAE J1772 standard for EVSE duty cycle
    } 
    else if (evse_duty_cycle > EVSE_DUTY_CYCLE_MID && evse_duty_cycle <= EVSE_DUTY_CYCLE_MAX)
    {
        return (evse_duty_cycle - 64.0f) * 2.5f;    // Formula derived from SAE J1772 standard for EVSE duty cycle  
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
