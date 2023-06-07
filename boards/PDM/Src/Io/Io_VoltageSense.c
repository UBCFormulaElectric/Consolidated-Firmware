#include "Io_VoltageSense.h"
#include "Io_Adc.h"

#define AMPLIFIER_GAIN (1.0f)
#define R_ERROR_COMPENSATION (1.0f)
#define VBAT_VOLTAGE_DIV (220e3f / (220e3f + 330e3f))
#define VBOOST_VOLTAGE_DIV (3.1e3f / (20e3f + 3.1e3f))

// TODO: Fix hard-coded values once the ADC is configured

float Io_VoltageSense_GetVbatVoltage(void)
{
    float adc_reading = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_10);
    return adc_reading * R_ERROR_COMPENSATION / (AMPLIFIER_GAIN * VBAT_VOLTAGE_DIV);
}

float Io_VoltageSense_Get24vAuxVoltage(void)
{
    float adc_reading = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_12);
    return adc_reading * R_ERROR_COMPENSATION / (AMPLIFIER_GAIN * VBOOST_VOLTAGE_DIV);
}

float Io_VoltageSense_Get24vAccVoltage(void)
{
    float adc_reading = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_11);
    return adc_reading * R_ERROR_COMPENSATION / (AMPLIFIER_GAIN * VBOOST_VOLTAGE_DIV);
}
