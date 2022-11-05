#include "main.h"
#include "Io_Brake.h"
#include "Io_Adc.h"

#include <math.h>

//TODO set these values
#define BRAKE_PEDAL_MIN_VOLTAGE (0.0f)
#define BRAKE_PEDAL_MAX_VOLTAGE (1.0f)

#include "configs/App_BrakePressureThresholds.h"
#define BRAKE_PRESSURE_SENSOR_MAX_V (5.0f)
#define BRAKE_PRESSURE_SC_THRESHOLD_V (4.6f)
#define BRAKE_PRESSURE_OC_THRESHOLD_V (0.4f)
#define BRAKE_PRESSURE_SC_THRESHOLD \
    (MAX_BRAKE_PRESSURE_PSI * BRAKE_PRESSURE_SC_THRESHOLD_V / BRAKE_PRESSURE_SENSOR_MAX_V)
#define BRAKE_PRESSURE_OC_THRESHOLD \
    (MAX_BRAKE_PRESSURE_PSI * BRAKE_PRESSURE_OC_THRESHOLD_V / BRAKE_PRESSURE_SENSOR_MAX_V)

//TODO should we keep this? This needs including "main.h"
bool Io_Brake_IsActuated(void)
{
    return HAL_GPIO_ReadPin(BSPD_BRAKE_STATUS_GPIO_Port, BSPD_BRAKE_STATUS_Pin) == GPIO_PIN_SET;
}

//pressure
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
    float front_pressure_voltage = Io_Adc_GetChannel3Voltage();
    if(Io_Brake_PressureVoltageAlarm(front_pressure_voltage)){
        return NAN;
    }
    return psi_per_volt * (front_pressure_voltage - min_input_voltage);
}
float Io_Brake_GetRearBrakePressurePsi(void)
{
    float rear_pressure_voltage = 0.5f;
    if(Io_Brake_PressureVoltageAlarm(rear_pressure_voltage)){
        return NAN;
    }
    //TODO calculate and return the pedal percentage travel
    return 0;
}
bool Io_Brake_PressureVoltageAlarm(float pressure_voltage){
    return !(BRAKE_PRESSURE_OC_THRESHOLD <= pressure_voltage && pressure_voltage <= BRAKE_PRESSURE_SC_THRESHOLD);
}

//pedal travel
float Io_Brake_GetPedalPercentTravel(void)
{
    float pedal_voltage = 0.5f;
    if(!(BRAKE_PEDAL_MIN_VOLTAGE <= pedal_voltage && pedal_voltage <= BRAKE_PEDAL_MAX_VOLTAGE)){
        return NAN;
    }
    //TODO calculate and return the pedal percentage travel
    return 0;
}
