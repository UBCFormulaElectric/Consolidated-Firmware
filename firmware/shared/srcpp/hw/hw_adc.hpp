#pragma once

#include "util_utils.hpp"
#include <array>
#include <cassert>

#ifdef TARGET_EMBEDDED
#include "main.h"
#endif

namespace hw
{
template <size_t NUM_ADC_CHANNELS> class AdcChip
{
    static constexpr float SINGLE_ENDED_ADC_V_SCALE = 3.3f;
    static constexpr float DIFFERENTIAL_ADC_V_SCALE = 6.6f;
    float                  rawAdcValueToVoltage(const bool is_differential, const uint16_t raw_adc_value) const
    {
        uint16_t full_scale;
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
        return scale * static_cast<float>(raw_adc_value) / static_cast<float>(full_scale);
    }

  private:
    ADC_HandleTypeDef *const                       hadc;
    TIM_HandleTypeDef *const                       htim;
    mutable std::array<float, NUM_ADC_CHANNELS>    adc_voltages;
    mutable std::array<uint16_t, NUM_ADC_CHANNELS> raw_adc_values;

  public:
    explicit AdcChip(ADC_HandleTypeDef *const in_hadc, TIM_HandleTypeDef *const in_htim)
      : hadc(in_hadc), htim(in_htim){};

    void init() const
    {
        assert(HAL_ADC_Start_DMA(hadc, (uint32_t *)raw_adc_values.data(), hadc->Init.NbrOfConversion) == HAL_OK);
        assert(HAL_TIM_Base_Start(htim) == HAL_OK);
    }

    void update_callback() const
    {
        for (uint16_t ch = 0; ch < NUM_ADC_CHANNELS; ch++)
            adc_voltages[ch] = rawAdcValueToVoltage(false, raw_adc_values[ch]);
    }
    [[nodiscard]] const float                 *getChannel(uint32_t channel) const { return &adc_voltages[channel]; }
    [[nodiscard]] constexpr ADC_HandleTypeDef *gethadc() const { return hadc; }
};

class Adc
{
    const float *const voltage_source;

  public:
    explicit Adc(const float *in_voltage_source) : voltage_source(in_voltage_source){};
    [[nodiscard]] float getVoltage() const { return *voltage_source; }
};
} // namespace hw