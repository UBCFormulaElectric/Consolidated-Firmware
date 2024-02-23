#pragma once

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
} AdcChannel;

/**
 * Get the buffer for DMA controller to write raw ADC values into.
 * @return Pointer to the buffer.
 */
uint16_t *hw_adc_getRawValuesBuffer(void);

/**
 * Get the voltage measured at an ADC channel.
 * @param channel The channel to get the voltage from.
 * @return The voltage measured at the specified ADC channel, in volts.
 */
float hw_adc_getVoltage(AdcChannel channel);
