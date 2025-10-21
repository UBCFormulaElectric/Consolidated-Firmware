#pragma once
#include "hw/hal.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    ADC_HandleTypeDef *const hadc;
    TIM_HandleTypeDef *const htim;

    uint16_t *raw_adc_values;
    float    *adc_voltages;
    uint16_t  channel_count;
    bool      is_differential;
} AdcChip;

/**
 * Initializes the given AdcChip (with DMA)
 * @param adc_c adc chip in question
 */
void hw_adcchip_init(const AdcChip *adc_c);

/**
 * Triggers the ADC chip to update it's values from the buffer
 * @param adc_c adc chip in question
 */
void hw_adcchip_updateCallback(const AdcChip *adc_c);

/**
 * Returns a pointer which represents the value of a given ADC channel, which is updated/populated by the chip
 * @param adc_c adc chip in question
 * @param channel the desired channel
 * @return A pointer to the value provided by the channel
 */
float *hw_adcchip_getChannel(const AdcChip *adc_c, uint32_t channel);

typedef struct
{
    float *voltage;
} AdcChannel;

/**
 * Get the voltage measured at an ADC channel.
 * @param c channel in question
 * @return The voltage measured at the specified ADC channel, in volts.
 */
float hw_adc_getVoltage(const AdcChannel *c);
