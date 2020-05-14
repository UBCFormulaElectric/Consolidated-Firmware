#include "App_SharedConstants.h"
#include "Io_SharedAdc.h"

float Convert_ADC_Voltage(ADC_HandleTypeDef *hadc, float ADC_Data)
{
    uint32_t full_scale;

    switch (hadc->Init.Resolution)
    {
        case ADC_RESOLUTION_6B:
            full_scale = MAX_6_BITS_VALUE;
            break;
        case ADC_RESOLUTION_8B:
            full_scale = MAX_8_BITS_VALUE;
            break;
        case ADC_RESOLUTION_10B:
            full_scale = MAX_10_BITS_VALUE;
            break;
        case ADC_RESOLUTION_12B:
            full_scale = MAX_12_BITS_VALUE;
            break;
    }

    return (3.3f / (float)full_scale) * ADC_Data;
}