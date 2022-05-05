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
    ADC1_CHANNEL_3,
    NUM_ADC1_CHANNELS
};

enum
{
    ADC2_CHANNEL_1,
    ADC2_CHANNEL_3,
    ADC2_CHANNEL_4,
    NUM_ADC2_CHANNELS
};

static uint16_t raw_adc1_values[NUM_ADC1_CHANNELS];
static uint16_t raw_adc2_values[NUM_ADC2_CHANNELS];
static float    adc1_voltages[NUM_ADC1_CHANNELS];
static float    adc2_voltages[NUM_ADC2_CHANNELS];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        adc1_voltages[ADC1_CHANNEL_3] =
            Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, true, (uint16_t)raw_adc1_values[ADC1_CHANNEL_3]);
    }
    else if (hadc->Instance == ADC2)
    {
        adc2_voltages[ADC2_CHANNEL_1] =
            Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc2_values[ADC2_CHANNEL_1]);
        adc2_voltages[ADC2_CHANNEL_3] =
            Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc2_values[ADC2_CHANNEL_3]);
        adc2_voltages[ADC2_CHANNEL_4] =
            Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc2_values[ADC2_CHANNEL_4]);
    }
}

uint16_t *Io_Adc_GetRawAdc1Values(void)
{
    return raw_adc1_values;
}

uint16_t *Io_Adc_GetRawAdc2Values(void)
{
    return raw_adc2_values;
}

float Io_Adc_GetAdc1Channel3Voltage(void)
{
    return adc1_voltages[ADC1_CHANNEL_3];
}

float Io_Adc_GetAdc2Channel1Voltage(void)
{
    return adc2_voltages[ADC2_CHANNEL_1];
}

float Io_Adc_GetAdc2Channel3Voltage(void)
{
    return adc2_voltages[ADC2_CHANNEL_3];
}

float Io_Adc_GetAdc2Channel4Voltage(void)
{
    return adc2_voltages[ADC2_CHANNEL_4];
}
