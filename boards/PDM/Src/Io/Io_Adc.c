#include <stm32f3xx.h>
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
// determines the order in which the DMA writes data to our raw_adc_values
// buffer.
//
// The following enum is used to index into raw_adc_values, which means it must
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
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    NUM_NORMAL_ADC_CHANNELS
};

enum
{
    CHANNEL_6,
    CHANNEL_7,
    CHANNEL_8,
    CHANNEL_9,
    NUM_INJECTED_ADC_CHANNELS
};

static uint16_t raw_normal_adc_values[NUM_NORMAL_ADC_CHANNELS];
static uint16_t raw_injected_adc_values[NUM_INJECTED_ADC_CHANNELS];
static float    normal_adc_voltages[NUM_NORMAL_ADC_CHANNELS];
static float    injected_adc_voltages[NUM_INJECTED_ADC_CHANNELS];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    normal_adc_voltages[CHANNEL_1] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
        hadc, raw_normal_adc_values[CHANNEL_1]);
    normal_adc_voltages[CHANNEL_2] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
        hadc, raw_normal_adc_values[CHANNEL_2]);
    normal_adc_voltages[CHANNEL_3] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
        hadc, raw_normal_adc_values[CHANNEL_3]);
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    raw_injected_adc_values[CHANNEL_6] =
        (uint16_t)HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
    raw_injected_adc_values[CHANNEL_7] =
        (uint16_t)HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_2);
    raw_injected_adc_values[CHANNEL_8] =
        (uint16_t)HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_3);
    raw_injected_adc_values[CHANNEL_9] =
        (uint16_t)HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_4);

    injected_adc_voltages[CHANNEL_6] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
        hadc, raw_injected_adc_values[CHANNEL_6]);
    injected_adc_voltages[CHANNEL_7] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
        hadc, raw_injected_adc_values[CHANNEL_7]);
    injected_adc_voltages[CHANNEL_8] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
        hadc, raw_injected_adc_values[CHANNEL_8]);
    injected_adc_voltages[CHANNEL_9] = Io_SharedAdc_ConvertRawAdcValueToVoltage(
        hadc, raw_injected_adc_values[CHANNEL_9]);
}
uint16_t *Io_Adc_GetRawNormalAdcValues(void)
{
    return raw_normal_adc_values;
}

uint16_t *Io_Adc_GetRawInjectedAdcValues(void)
{
    return raw_injected_adc_values;
}

float Io_Adc_GetChannel1Voltage(void)
{
    return normal_adc_voltages[CHANNEL_1];
}

float Io_Adc_GetChannel2Voltage(void)
{
    return normal_adc_voltages[CHANNEL_2];
}

float Io_Adc_GetChannel3Voltage(void)
{
    return normal_adc_voltages[CHANNEL_3];
}

float Io_Adc_GetChannel6Voltage(void)
{
    return injected_adc_voltages[CHANNEL_6];
}

float Io_Adc_GetChannel7Voltage(void)
{
    return injected_adc_voltages[CHANNEL_7];
}

float Io_Adc_GetChannel8Voltage(void)
{
    return injected_adc_voltages[CHANNEL_8];
}

float Io_Adc_GetChannel9Voltage(void)
{
    return injected_adc_voltages[CHANNEL_9];
}