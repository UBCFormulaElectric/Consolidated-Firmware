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

namespace hw::adcs
{
extern AdcChip<6> adc1;

void chipsInit();

extern Adc susp_fl;
extern Adc susp_fr;
extern Adc apps2;
extern Adc bps_f;
extern Adc str_angle;
extern Adc apps1;
} // namespace hw::adcs
