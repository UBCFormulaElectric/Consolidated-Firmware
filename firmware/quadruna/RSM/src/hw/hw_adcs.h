#pragma once

#include "hw_adc.h"

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

extern const AdcChannel lc4;
extern const AdcChannel susp_travel_rl;
extern const AdcChannel coolant_temp_1;
extern const AdcChannel coolant_temp_2;
extern const AdcChannel susp_travel_rr;
extern const AdcChannel lc3;
extern const AdcChannel coolant_pressure_2;
extern const AdcChannel coolant_pressure_1;
extern const AdcChannel rad_fan_i_sns;
extern const AdcChannel acc_fan_i_sns;

void hw_adcs_chipsInit();
