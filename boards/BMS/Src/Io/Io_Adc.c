#include "Io_Hal.h"
#include "Io_SharedAdc.h"
#include "Io_Adc.h"

// In STM32 terminology, each ADC pin corresponds to an ADC channel (See:
// ADCEx_channels). If there are multiple ADC channels being measured, the ADC
// measures them based on "rank" (See: ADCEx_regular_rank). The rank starts
// counting at 1, and the lower the rank, the higher the measurement priority.
//
// For example, suppose we are measuring ADC channel 2, 4, and 7, which have
// rank 3, 1, and 2 respectively. The ADC will measure the channel 4, then
// channel 7, and finally channel 2. This order is important because it
// determines the order in which the DMA writes data to our raw_adc2_values
// buffer.
//
// The following enum is used to index into raw_adc2_values, which means it must
// be ordered in ascending ranks. If we were writing an enum for the earlier
// example, it would look like:
//
// enum
// {
//     CHANNEL_4, // Rank 1
//     CHANNEL_7, // Rank 2
//     CHANNEL_2, // Rank 3
//     NUM_ADC_CHANNELS,
// };

enum
{
    ADC1_CHANNEL_3,  // SHDN_ISENSE
    ADC1_CHANNEL_7,  // EVSE_PROX_PILOT
    ADC1_CHANNEL_8,  // TS_ISENSE_50A
    ADC1_CHANNEL_9,  // TS_ISENSE_300A
    ADC1_CHANNEL_10, // TS_VSENSE_P
    ADC1_CHANNEL_11, // TS_VSENSE_N
    ADC1_CHANNEL_14, // TEMP_SENSE
    NUM_ADC1_CHANNELS
};

static uint16_t raw_adc_values[NUM_ADC1_CHANNELS];
static float    adc_voltages[NUM_ADC1_CHANNELS];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_voltages[ADC1_CHANNEL_3] =
        Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_3]);
    adc_voltages[ADC1_CHANNEL_7] =
        Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_7]);
    adc_voltages[ADC1_CHANNEL_8] =
        Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_8]);
    adc_voltages[ADC1_CHANNEL_9] =
        Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_9]);
    adc_voltages[ADC1_CHANNEL_10] =
        Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, true, (uint16_t)raw_adc_values[ADC1_CHANNEL_10]);
    adc_voltages[ADC1_CHANNEL_11] =
        Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_11]);
    adc_voltages[ADC1_CHANNEL_14] =
        Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[ADC1_CHANNEL_14]);
}

uint16_t *Io_Adc_GetRawAdcValues(void)
{
    return raw_adc_values;
}

float Io_Adc_GetAdcChannel3Voltage(void)
{
    return adc_voltages[ADC1_CHANNEL_3];
}

float Io_Adc_GetAdcChannel7Voltage(void)
{
    return adc_voltages[ADC1_CHANNEL_7];
}

float Io_Adc_GetAdcChannel8Voltage(void)
{
    return adc_voltages[ADC1_CHANNEL_8];
}

float Io_Adc_GetAdcChannel9Voltage(void)
{
    return adc_voltages[ADC1_CHANNEL_9];
}

float Io_Adc_GetAdcChannel10Voltage(void)
{
    return adc_voltages[ADC1_CHANNEL_10];
}

float Io_Adc_GetAdcChannel11Voltage(void)
{
    return adc_voltages[ADC1_CHANNEL_11];
}

float Io_Adc_GetAdcChannel14Voltage(void)
{
    return adc_voltages[ADC1_CHANNEL_14];
}
