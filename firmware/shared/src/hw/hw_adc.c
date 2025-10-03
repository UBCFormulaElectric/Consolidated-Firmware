#include "hw_adc.h"

#include "app_utils.h"
#include "hw_utils.h"

#define SINGLE_ENDED_ADC_V_SCALE (3.3f)
#define DIFFERENTIAL_ADC_V_SCALE (6.6f)

static bool seen_error = false;

static float
    rawAdcValueToVoltage(const ADC_HandleTypeDef *hadc, const bool is_differential, const uint16_t raw_adc_value)
{
    uint16_t full_scale = MAX_12_BITS_VALUE;

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
        default:
            full_scale = MAX_12_BITS_VALUE;
            break;
    }

    // Taken from the STM32 manual, the formula to convert the raw ADC
    // measurements to an absolute voltage value is as follows:
    //
    //                   V_SCALE (VDDA or 2VDDA) x ADC_DATAx
    //    V_CHANNELx = ---------------------------------------
    //                               FULL_SCALE
    // where:
    // - V_SCALE depends on whether the ADC is configured as a single ended or
    // differential input
    // - ADC_DATAx is the value measured by the ADC on channel x (right-aligned)
    // - FULL_SCALE is the maximum digital value of the ADC output. For example
    //   with 12-bit resolution, it will be 2^12 -1 = 4095 or with 8-bit
    //   resolution, 2^8 - 1 = 255.

    const float scale = (is_differential) ? DIFFERENTIAL_ADC_V_SCALE : SINGLE_ENDED_ADC_V_SCALE;

    // Offsets to the raw ADC value should be configured via cube for
    // differential ADC mode
    return scale * (float)raw_adc_value / (float)full_scale;
}

void hw_adcchip_init(const AdcChip *adc_c)
{
    LOG_IF_ERR(hw_utils_convertHalStatus(
        HAL_ADC_Start_DMA(adc_c->hadc, (uint32_t *)adc_c->raw_adc_values, adc_c->hadc->Init.NbrOfConversion)));
    HAL_TIM_Base_Start(adc_c->htim);
}

void hw_adcchip_updateCallback(const AdcChip *adc_c)
{
    seen_error = false;
    for (uint16_t ch = 0; ch < adc_c->channel_count; ch++)
        adc_c->adc_voltages[ch] = rawAdcValueToVoltage(adc_c->hadc, adc_c->is_differential, adc_c->raw_adc_values[ch]);
}

float *hw_adcchip_getChannel(const AdcChip *adc_c, const uint32_t channel)
{
    return &adc_c->adc_voltages[channel];
}

float hw_adc_getVoltage(const AdcChannel *c)
{
    return *c->voltage;
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    if (seen_error)
        return;

    switch (hadc->ErrorCode)
    {
        case HAL_ADC_ERROR_NONE:
            LOG_ERROR("ADC Error: None");
            break;
        case HAL_ADC_ERROR_INTERNAL:
            LOG_ERROR("ADC Error: Internal");
            break;
        case HAL_ADC_ERROR_OVR:
            LOG_ERROR("ADC Error: Overrun");
            break;
        case HAL_ADC_ERROR_DMA:
            LOG_ERROR("ADC Error: DMA");
            break;
        default:
            LOG_ERROR("ADC Error: Unknown, code %lu", hadc->ErrorCode);
            break;
    }
    seen_error = true;
}

void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)
{
    LOG_ERROR("Analog watchdog 1 callback in non-blocking mode.");
}