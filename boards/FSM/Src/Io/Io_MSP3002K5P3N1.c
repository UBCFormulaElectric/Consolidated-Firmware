#include "Io_Adc.h"
#include "main.h"
#include "Io_MSP3002K5P3N1.h"

bool Io_MSP3002K5P3N1_IsOpenOrShortCircuit(void)
{
    return HAL_GPIO_ReadPin(BRAKE_OC_SC_OK_GPIO_Port, BRAKE_OC_SC_OK_Pin) == GPIO_PIN_RESET;
}

float Io_MSP3002K5P3N1_GetPressurePsi(void)
{
    // The sensor operates from 0.5V to 4.5V. The voltage divider decreases the
    // voltage by a factor of (2/3). Thus the minimum voltage seen by the analog
    // input pin is 0.33V while the maximum voltage seen is 3V
    const float MIN_INPUT_VOLTAGE = 0.33f;
    const float MAX_INPUT_VOLTAGE = 3.0f;

    // Psi Per Volt = (Max Pressure - Min Pressure) / (Max Input Voltage - Min
    // Input Voltage)
    const float PSI_PER_VOLT = 2500.0f / (MAX_INPUT_VOLTAGE - MIN_INPUT_VOLTAGE);

    // Brake pressure = (ADC Voltage - Min Input Voltage) * Psi Per Volt
    return PSI_PER_VOLT * (Io_Adc_GetChannel3Voltage() - MIN_INPUT_VOLTAGE);
}
