#include "Io_CurrentSense.h"
#include "Io_Adc.h"
#include "main.h"

float Io_CurrentSense_GetAux1Current(void)
{
    // Aux1 Current = ADC Voltage * Current Gain Ratio

    const float LOW_CURRENT_SENSE_GAIN_RATIO = 500.0f;
    return (Io_Adc_GetChannelVoltage(ADC1_CHANNEL_6) * LOW_CURRENT_SENSE_GAIN_RATIO);
}

float Io_CurrentSense_GetAux2Current(void)
{
    // Aux2 Current = ADC Voltage * Current Gain Ratio

    const float LOW_CURRENT_SENSE_GAIN_RATIO = 500.0f;
    return (Io_Adc_GetChannelVoltage(ADC1_CHANNEL_6) * LOW_CURRENT_SENSE_GAIN_RATIO);
}

float Io_CurrentSense_GetLeftInverterCurrent(void)
{
    return 0.5f;
}

float Io_CurrentSense_GetRightInverterCurrent(void)
{
    return 0.5f;
}

float Io_CurrentSense_GetEnergyMeterCurrent(void)
{
    return 0.5f;
}

float Io_CurrentSense_GetCanCurrent(void)
{
    return 0.5f;
}

float Io_CurrentSense_GetAirShutdownCurrent(void)
{
    return 0.5f;
}
