#include "main.h"
#include "Io_Brake.h"
#include "Io_Adc.h"

// TODO set these values
#define BRAKE_PEDAL_MIN_VOLTAGE (0.5f)
#define BRAKE_PEDAL_MAX_VOLTAGE (4.5f)

#define BRAKE_PRESSURE_OC_THRESHOLD_V (0.33f)
#define BRAKE_PRESSURE_SC_THRESHOLD_V (3.0f)
#define BRAKE_PRESSURE_SENSOR_MAX_V (5.0f)

bool Io_Brake_IsActuated(void)
{
    return HAL_GPIO_ReadPin(BSPD_BRAKE_PRESSED_5V_GPIO_Port, BSPD_BRAKE_PRESSED_5V_Pin) == GPIO_PIN_SET;
}
bool Io_Brake_PressureVoltageAlarm(float pressure_voltage)
{
    return !(BRAKE_PRESSURE_OC_THRESHOLD_V <= pressure_voltage && pressure_voltage <= BRAKE_PRESSURE_SC_THRESHOLD_V);
}

float BrakePressureFromVoltage(float voltage)
{
    // The sensor operates from 0.5V to 4.5V. The voltage divider decreases the
    // voltage by a factor of (2/3). Thus the minimum voltage seen by the analog
    // input pin is 0.33V while the maximum voltage seen is 3V
    const float min_input_voltage = BRAKE_PRESSURE_OC_THRESHOLD_V;
    const float max_input_voltage = BRAKE_PRESSURE_SC_THRESHOLD_V;

    // https://www.mouser.ca/datasheet/2/418/8/ENG_DS_MSP300_B1-1130121.pdf
    // Psi Per Volt = (Max Pressure - Min Pressure) / (Max Input Voltage - Min Input Voltage)
    const float psi_per_volt = 2500.0f / (max_input_voltage - min_input_voltage);

    // Brake pressure = (ADC Voltage - Min Input Voltage) * Psi Per Volt
    return psi_per_volt * (voltage - min_input_voltage);
}

float Io_Brake_GetFrontPressureSensorVoltage(void)
{
    return Io_Adc_GetChannelVoltage(ADC_CHANNEL_7);
}
float Io_Brake_GetFrontPressurePsi(void)
{
    return BrakePressureFromVoltage(Io_Brake_GetFrontPressureSensorVoltage());
}
bool Io_Brake_FrontPressureSensorOCSC(void)
{
    float front_pressure_voltage = Io_Brake_GetFrontPressureSensorVoltage();
    return Io_Brake_PressureVoltageAlarm(front_pressure_voltage);
}

float Io_Brake_GetRearPressureSensorVoltage(void)
{
    return Io_Adc_GetChannelVoltage(ADC_CHANNEL_8);
}
float Io_Brake_GetRearPressurePsi(void)
{
    return BrakePressureFromVoltage(Io_Brake_GetRearPressureSensorVoltage());
}
bool Io_Brake_RearPressureSensorOCSC(void)
{
    float rear_pressure_voltage = Io_Brake_GetRearPressureSensorVoltage();
    return Io_Brake_PressureVoltageAlarm(rear_pressure_voltage);
}

// pedal travel
float Io_Brake_GetPedalSensorVoltage(void)
{
    return Io_Adc_GetChannelVoltage(ADC_CHANNEL_9);
}
float Io_Brake_GetPedalPercentTravel(void)
{
    float       pedal_voltage    = Io_Brake_GetPedalSensorVoltage();
    const float percent_per_volt = 100 / (BRAKE_PEDAL_MAX_VOLTAGE - BRAKE_PEDAL_MIN_VOLTAGE);
    return (pedal_voltage - BRAKE_PEDAL_MIN_VOLTAGE) * percent_per_volt;
}
bool Io_Brake_PedalSensorOCSC(void)
{
    return !(
        BRAKE_PEDAL_MIN_VOLTAGE <= Io_Brake_GetPedalSensorVoltage() &&
        Io_Brake_GetPedalSensorVoltage() <= BRAKE_PEDAL_MAX_VOLTAGE);
}
