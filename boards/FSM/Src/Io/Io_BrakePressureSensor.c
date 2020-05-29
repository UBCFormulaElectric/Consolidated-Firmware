#include <stdbool.h>
#include "Io_Adc.h"

float Io_BrakePressureSensor_GetPressurePsi(void)
{
    // Brake pressure = (ADC Voltage - Voltage Offset) * psi per volt
    const float BRAKE_PRESSURE_VOLTAGE_OFFSET = 0.33f;
    const float PSI_PER_VOLT                  = 2500.0f / 3.3f;

    return PSI_PER_VOLT *
           (Io_Adc_GetChannel3Voltage() - BRAKE_PRESSURE_VOLTAGE_OFFSET);
}
