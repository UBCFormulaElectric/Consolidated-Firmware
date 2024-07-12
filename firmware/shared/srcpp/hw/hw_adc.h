#pragma once

#include "hw_hal.h"
#include "main.h"
#include "app_utils.h"
#include <array>

namespace hw
{
template <size_t NUM_ADC_CHANNELS>
class AdcChip
{
#define SINGLE_ENDED_ADC_V_SCALE (3.3f)
#define DIFFERENTIAL_ADC_V_SCALE (6.6f)
    ADC_HandleTypeDef* const hadc;
    std::array<uint16_t, NUM_ADC_CHANNELS> raw_adc_values;
    std::array<float, NUM_ADC_CHANNELS> adc_voltages;

    float rawAdcValueToVoltage(bool is_differential, uint16_t raw_adc_value)
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

  public:
    explicit AdcChip(ADC_HandleTypeDef* const in_hadc): hadc(in_hadc) {};

    void init() const {
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)raw_adc_values.data(), hadc1.Init.NbrOfConversion);
        HAL_TIM_Base_Start(&htim3);
    }

    [[nodiscard]] const float * getChannel(uint32_t channel) const {
        return &adc_voltages[channel];
    }
};

class Adc
{
    const float * const voltage_source;
  public:
    explicit Adc(const float * voltage_source): voltage_source(voltage_source) {};
    float getVoltage() {
        return *voltage_source;
    }
};
} // namespace hw