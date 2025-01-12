#pragma once
#include "hw_adc.h"
#include <stdint.h>

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

extern const AdcChannel acc_fan_i_sns;
extern const AdcChannel rad_fan_i_sns;
extern const AdcChannel coolantpressure1_3v3;
extern const AdcChannel coolantpressure2_3v3;
extern const AdcChannel lc3_out;
extern const AdcChannel susp_travel_rr_3v3;
extern const AdcChannel susp_travel_rl_3v3;
extern const AdcChannel coolanttemp2_3v3;
extern const AdcChannel coolanttemp1_3v3;
extern const AdcChannel regen_3v3_lc3_out;