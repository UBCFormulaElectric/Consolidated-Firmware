#pragma once
#include "hw_adc.h"
#include <stdint.h>

namespace hw::adc
{


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
//
// The following enum is used to index into raw_adc_values, which means it must
// be ordered in ascending ranks. If we were writing an enum for the earlier
// example, it would look like:
//
// enum
// {
//     CHANNEL_4, // Rank 1
//     CHANNEL_7, // Rank 2
//     CHANNEL_2, // Rank 3
//     NUM_ADC_CHANNELS,
// };

enum AdcChannel
{
    ADC1_IN0_LC4_OUT,
    ADC1_IN1_SUSP_TRAVEL_RL,
    ADC1_IN2_COOLANT_TEMP_1,
    ADC1_IN3_COOLANT_TEMP_2,
    ADC1_IN4_SUSP_TRAVEL_RR,
    ADC1_IN10_LC3_OUT,
    ADC1_IN11_COOLANT_PRESSURE_2,
    ADC1_IN12_COOLANT_PRESSURE_1,
    ADC1_IN14_RAD_FAN_I_SNS,
    ADC1_IN15_ACC_FAN_I_SNS,
    NUM_ADC_CHANNELS
};

extern const AdcChip<NUM_ADC_CHANNELS> adc1;
extern const Adc              acc_fan_i_sns;
extern const Adc              rad_fan_i_sns;
extern const Adc              coolantpressure1_3v3;
extern const Adc              coolantpressure2_3v3;
extern const Adc              lc3_out;
extern const Adc              susp_travel_rr_3v3;
extern const Adc              susp_travel_rl_3v3;
extern const Adc              coolanttemp2_3v3;
extern const Adc              coolanttemp1_3v3;
extern const Adc              regen_3v3_lc3_out;
}



