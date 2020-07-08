#include "Io_Adc.h"
#include "Io_VoltageSense.h"

float Io_VoltageSense_GetVbatVoltage(void)
{
    // Vbat Voltage = ADC Voltage * Voltage Gain Ratio

    const float VBAT_VOLTAGE_GAIN_RATIO = (float)220.0e6 / (float)(220.0e6 + 330.0e6);
    return (Io_Adc_GetChannel1Voltage() * VBAT_VOLTAGE_GAIN_RATIO);
}

float Io_VoltageSense_Get24vAccVoltage(void)
{
    // Acc Voltage = ADC Voltage * Voltage Gain Ratio

    const float ACC_VOLTAGE_GAIN_RATIO = (float)150.0e6 / (float)(150.0e6 + 1.0e9);
    return (Io_Adc_GetChannel2Voltage() * ACC_VOLTAGE_GAIN_RATIO);
}

float Io_VoltageSense_Get24vBoostOutVoltage(void)
{
    // Boost Out Voltage = ADC Voltage * Voltage Gain Ratio

    const float BOOST_OUT_VOLTAGE_GAIN_RATIO = (float)150.0e6 / (float)(150.0e6 + 1.0e9);
    return (Io_Adc_GetChannel2Voltage() * BOOST_OUT_VOLTAGE_GAIN_RATIO);
}
