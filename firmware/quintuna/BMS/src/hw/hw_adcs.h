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

extern const AdcChannel fan_isns;
extern const AdcChannel ts_vsense_p;
extern const AdcChannel ts_vsense_n;
extern const AdcChannel shdn_isns;
extern const AdcChannel emeter_tsns;
extern const AdcChannel ts_isns_400a;
extern const AdcChannel ts_isns_50a;
extern const AdcChannel aux_tsns;

void hw_adcs_chipsInit(void);
