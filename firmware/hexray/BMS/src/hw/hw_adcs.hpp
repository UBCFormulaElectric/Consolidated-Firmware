#pragma once

#include "hw_adc.hpp"

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

extern const hw::AdcChip<7> adc1;
extern const hw::AdcChip<1> adc3;

extern const hw::Adc fan_isns;
extern const hw::Adc ts_vsense_p;
extern const hw::Adc ts_vsense_n;
extern const hw::Adc shdn_sns;
extern const hw::Adc emeter_tsns;
extern const hw::Adc ts_isense_400a;
extern const hw::Adc ts_isense_50a;
extern const hw::Adc aux_tsns;

inline void adcChipsInit()
{
    LOG_IF_ERR(adc1.init());
    LOG_IF_ERR(adc3.init(false));
}
