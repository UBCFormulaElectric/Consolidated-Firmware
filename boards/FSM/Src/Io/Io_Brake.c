#include "main.h"
#include "Io_Brake.h"
#include "Io_Adc.h"

bool Io_Brake_IsActuated(void)
{
    return HAL_GPIO_ReadPin(BSPD_BRAKE_STATUS_GPIO_Port, BSPD_BRAKE_STATUS_Pin) == GPIO_PIN_SET;
}

bool Io_Brake_IsFrontBrakeOpenOrShortCircuit(void)
{
    return HAL_GPIO_ReadPin(BRAKE_OC_SC_OK_GPIO_Port, BRAKE_OC_SC_OK_Pin) == GPIO_PIN_RESET;
}

float Io_Brake_GetFrontBrakePressurePsi(void)
{
    // The sensor operates from 0.5V to 4.5V. The voltage divider decreases the
    // voltage by a factor of (2/3). Thus the minimum voltage seen by the analog
    // input pin is 0.33V while the maximum voltage seen is 3V
    const float min_input_voltage = 0.33f;
    const float max_input_voltage = 3.0f;

    // Psi Per Volt = (Max Pressure - Min Pressure) / (Max Input Voltage - Min
    // Input Voltage)
    const float psi_per_volt = 2500.0f / (max_input_voltage - min_input_voltage);

    // Brake pressure = (ADC Voltage - Min Input Voltage) * Psi Per Volt
    return psi_per_volt * (Io_Adc_GetChannel3Voltage() - min_input_voltage);
}

// TODO Implement the IO rear brake open/short circuit function
bool Io_Brake_IsRearBrakeOpenOrShortCircuit(void)
{
    return false;
}
// TODO Implement the IO rear brake pressure PSI Function
float Io_Brake_GetRearBrakePressurePsi(void)
{
    return 1.6f;
}

// TODO Implement the IO brake pedal angle open/short circuit Function
bool IO_Brake_IsPedalOpenOrShortCircuit(void)
{
    return false;
}
// TODO Implement the IO brake pedal angle Function
float Io_Brake_GetPedalPercentTravel(void)
{
    return 0;
}
