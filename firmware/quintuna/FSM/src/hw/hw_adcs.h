#pragma once
#include "hw_adc.h"

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
typedef enum
{
    ADC1_IN1_LOAD_CELL_2,
    ADC1_IN5_APPS2,
    ADC1_IN7_BPS_F,
    ADC1_IN8_SUSP_TRAVEL_FL,
    ADC1_IN9_SUSP_TRAVEL_FR,
    ADC1_IN11_STEERING_ANGLE,
    ADC1_IN12_APPS1,
    ADC1_IN13_LOAD_CELL_1,
    ADC1_IN15_BPS_B,
    NUM_ADC_CHANNELS
} AdcChannel;

extern const AdcChip<NUM_ADC_CHANNELS> adc1;
extern const Adc                       load_cell_1_adc;
extern const Adc                       apps2_adc;
extern const Adc                       break_pres_sensor_f_adc;
extern const Adc                       break_pres_sensor_b_adc;
extern const Adc                       apps1_adc;
extern const Adc                       load_cell_2_adc;
extern const Adc                       susp_travel_fl_adc;
extern const Adc                       susp_travel_fr_adc;
extern const Adc                       steering_angle
} // namespace hw::adc