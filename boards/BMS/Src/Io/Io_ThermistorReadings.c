#include "Io_ThermistorReadings.h"
#include "Io_Hal.h"
#include "Io_Adc.h"
#include "main.h"

static void intToBinaryArray(uint8_t input_int, GPIO_PinState *output_arr)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        output_arr[i] = GPIO_PIN_RESET;
    }

    if (input_int >= 8)
    {
        output_arr[3] = GPIO_PIN_SET;
        input_int     = (uint8_t)(input_int - 8U);
    }
    if (input_int >= 4)
    {
        output_arr[2] = GPIO_PIN_SET;
        input_int     = (uint8_t)(input_int - 4U);
    }
    if (input_int >= 2)
    {
        output_arr[1] = GPIO_PIN_SET;
        input_int     = (uint8_t)(input_int - 2U);
    }
    if (input_int == 1)
    {
        output_arr[0] = GPIO_PIN_SET;
    }
}

void Io_ThermistorReadings_MuxSelect(uint8_t channel)
{
    GPIO_PinState binary_select[4];

    intToBinaryArray(channel, binary_select);

    HAL_GPIO_WritePin(AUX_TSENSE_MUX0_GPIO_Port, AUX_TSENSE_MUX0_Pin, binary_select[0]);
    HAL_GPIO_WritePin(AUX_TSENSE_MUX1_GPIO_Port, AUX_TSENSE_MUX1_Pin, binary_select[1]);
    HAL_GPIO_WritePin(AUX_TSENSE_MUX2_GPIO_Port, AUX_TSENSE_MUX2_Pin, binary_select[2]);
    HAL_GPIO_WritePin(AUX_TSENSE_MUX3_GPIO_Port, AUX_TSENSE_MUX3_Pin, binary_select[3]);
}

float Io_ThermistorReadings_ReadSelectedTemp(void)
{
    float raw_voltage = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_14);

    // TODO: Convert voltage into temperature
    return raw_voltage;
}
