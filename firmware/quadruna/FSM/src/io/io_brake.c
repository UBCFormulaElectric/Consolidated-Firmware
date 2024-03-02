#include "main.h"
#include "io_brake.h"
#include "hw_adc.h"

// TODO set these values
#define BRAKE_PEDAL_MIN_VOLTAGE (2.08f)
#define BRAKE_PEDAL_MAX_VOLTAGE (2.310f)

#define BRAKE_PRESSURE_PRESSED_THRESHOLD_PSI (40.0f)
#define BRAKE_PRESSURE_OC_THRESHOLD_V (0.33f)
#define BRAKE_PRESSURE_SC_THRESHOLD_V (3.0f)
#define BRAKE_PRESSURE_SENSOR_MAX_V (5.0f)

static bool pressureSensorOCSC(float pressure_voltage)
{
    return !(BRAKE_PRESSURE_OC_THRESHOLD_V <= pressure_voltage && pressure_voltage <= BRAKE_PRESSURE_SC_THRESHOLD_V);
}

static float pressureFromVoltage(float voltage)
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

bool io_brake_isActuated(void)
{
    return io_brake_getFrontPressurePsi() > BRAKE_PRESSURE_PRESSED_THRESHOLD_PSI ||
           io_brake_getRearPressurePsi() > BRAKE_PRESSURE_PRESSED_THRESHOLD_PSI;
}

float io_brake_getFrontPressurePsi(void)
{
    return pressureFromVoltage(hw_adc_getVoltage(ADC1_IN7_BPS_F));
}

bool io_brake_frontPressureSensorOCSC(void)
{
    float front_pressure_voltage = hw_adc_getVoltage(ADC1_IN7_BPS_F);
    return pressureSensorOCSC(front_pressure_voltage);
}

float io_brake_getRearPressurePsi(void)
{
    return pressureFromVoltage(hw_adc_getVoltage(ADC1_IN15_BPS_B));
}

bool io_brake_rearPressureSensorOCSC(void)
{
    float rear_pressure_voltage = hw_adc_getVoltage(ADC1_IN15_BPS_B);
    return pressureSensorOCSC(rear_pressure_voltage);
}