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
    CHANNEL_6,
    CHANNEL_7,
    CHANNEL_8,
    CHANNEL_9,
    NUM_ADC_CHANNELS
};

static uint16_t raw_adc_values[NUM_ADC_CHANNELS];
static float    adc_voltages[NUM_ADC_CHANNELS];

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_voltages[CHANNEL_1] = Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[CHANNEL_1]);
    adc_voltages[CHANNEL_2] = Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[CHANNEL_2]);
    adc_voltages[CHANNEL_3] = Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[CHANNEL_3]);
    adc_voltages[CHANNEL_6] = Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[CHANNEL_6]);
    adc_voltages[CHANNEL_7] = Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[CHANNEL_7]);
    adc_voltages[CHANNEL_8] = Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[CHANNEL_8]);
    adc_voltages[CHANNEL_9] = Io_SharedAdc_ConvertRawAdcValueToVoltage(hadc, false, raw_adc_values[CHANNEL_9]);
}

uint16_t *Io_Adc_GetRawAdcValues(void)
{
    return raw_adc_values;
}

float Io_Adc_GetChannel1Voltage(void)
{
    return adc_voltages[CHANNEL_1];
}

float Io_Adc_GetChannel2Voltage(void)
{
    return adc_voltages[CHANNEL_2];
}

float Io_Adc_GetChannel3Voltage(void)
{
    return adc_voltages[CHANNEL_3];
}

float Io_Adc_GetChannel6Voltage(void)
{
    return adc_voltages[CHANNEL_6];
}

float Io_Adc_GetChannel7Voltage(void)
{
    return adc_voltages[CHANNEL_7];
}

float Io_Adc_GetChannel8Voltage(void)
{
    return adc_voltages[CHANNEL_8];
}

float Io_Adc_GetChannel9Voltage(void)
{
    return adc_voltages[CHANNEL_9];
}
