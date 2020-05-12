#pragma once
#include <stm32f3xx_hal.h>
#include "Io_SharedAdcConfig.h"

#ifndef MAX_NUM_OF_ADC
#define MAX_NUM_OF_ADC 1
#endif
#ifndef MAX_NUM_OF_ADC_CHANNELS
#define MAX_NUM_OF_ADC_CHANNELS 16
#endif

struct AdcInput;

/**
 * Allocate and initialize an ADC instance with direct memory access
 *
 * @note The user must ensure that only one of the two ADCs is converting
 * VREFINT as it is forbidden to have several ADCs converting
 * VREFINT at the same time. Create the ADC instance containing VREFINT first
 * while initializing the ADCs
 *
 * @note To set up the given ADC with a DMA controller, it must be initialized
 * with (1) Continuous Conversion Mode enabled,(2) DMA Continuous Requests
 * enabled, (3) DMA buffer with Circular Mode selected
 *
 * @param adc_handle The handle for the given ADC
 * @param vrefint_rank The rank which is used to indicate the index of VREFINT
 * @return A pointer to the allocated and initialized ADC instance
 */
struct AdcInput *
    Io_SharedAdc_Init(ADC_HandleTypeDef *adc_handle, size_t vrefint_rank);

/**
 * Gets the ADC voltage of the given channel from the given ADC instance
 * @param adc_input The given ADC instance to get the voltage from
 * @param adc_rank The rank of the ADC channel to get the voltage from
 * @return The voltage of the given ADC handle's given channel
 */
float Io_SharedAdc_GetChannelVoltage(
    const struct AdcInput *adc_input,
    size_t                 adc_rank);

/**
 * Gets the pointer to the adc handle from the given ADC instance
 * @param adc_input The given ADC instance to acquire the adc handle from
 * @return The ADC_HandleTypedef structure for the specific ADC
 */
ADC_HandleTypeDef *Io_SharedAdc_GetAdcHandle(const struct AdcInput *adc_input);

/**
 * Gets the number of active channels from the given ADC instance
 * @param adc_input Handle for the given ADC instance
 * @return The number of active channels for the given ADC instance
 */
uint32_t Io_SharedAdc_GetNumActiveChannel(const struct AdcInput *adc_input);
