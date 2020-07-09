#include "Io_CurrentSense.h"
#include "Io_Adc.h"
#include "main.h"

// TODO: Fix hard-coded values once the ADC is configured to read load-switches
extern ADC_HandleTypeDef hadc1;

float Io_CurrentSense_GetAux1Current(void)
{
    // Aux1 Current = ADC Voltage * Current Gain Ratio

    const float LOW_CURRENT_SENSE_GAIN_RATIO = 500.0f;

    HAL_ADCEx_InjectedStart_IT(&hadc1);

    return (LOW_CURRENT_SENSE_GAIN_RATIO * Io_Adc_GetChannel6Voltage());
}

float Io_CurrentSense_GetAux2Current(void)
{
    return 0.5f;
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
