#include <cstdint>
#include <cmath>

#include "app_charger.hpp"
#include "io_charger.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

namespace app::charger
{
// UNUSED
// const float getOutputVoltage()
// {
//     return app::can_rx::Elcon_OutputVoltage_get();
// }

float getOutputCurrent()
{
    return app::can_rx::Elcon_OutputCurrent_get();
}

// Read current charger status from CAN→typed struct
ElconFaultConfig getFaultStatus()
{
    ElconFaultConfig rx{ .hardware_failure     = app::can_rx::Elcon_HardwareFailure_get(),
                         .over_temperature     = app::can_rx::Elcon_ChargerOverTemperature_get(),
                         .input_voltage_fault  = app::can_rx::Elcon_InputVoltageError_get(),
                         .charging_state_fault = app::can_rx::Elcon_ChargingDisabled_get(),
                         .comm_timeout         = app::can_rx::Elcon_CommunicationTimeout_get() };
    return rx;
}

// Push a charging command to the BMS CAN TX table
void setChargingConfig(const ElconChargingConfig &cmd)
{
    app::can_tx::BMS_MaxChargingVoltage_set(cmd.max_voltage_v);
    app::can_tx::BMS_MaxChargingCurrent_set(cmd.max_current_a);
    app::can_tx::BMS_StopCharging_set(cmd.stop_charging);
}

float getAvailableCurrent()
{
    // It is not directly related to this module, but we do not expect to ever exceed 16A from the
    // AC circuit at competition due to breaker limits + cell constraints + charger limits.
    // As a result, we do not expect to exceed ~35% duty cycle on evse under any circumstance.
    const float evse_duty_cycle = io::charger::getCPDutyCycle();

    if (evse_duty_cycle >= 10.0f && evse_duty_cycle <= 85.0f)
    {
        return evse_duty_cycle * 0.6f;
    }
    else if (evse_duty_cycle > 85.0f && evse_duty_cycle <= 97.0f)
    {
        return evse_duty_cycle * 2.5f - 64.0f;
    }

    return 0.0f; // Sentinel value for invalid/critical conditions
}
} // namespace app::charger