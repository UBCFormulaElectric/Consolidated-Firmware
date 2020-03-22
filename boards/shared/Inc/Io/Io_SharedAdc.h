#pragma once

#include <stm32f3xx_hal.h>
#include <arm_math.h>

#include "Io_BoardSpecifics.h"

// clang-format off

// Memory address where you can find the internal reference voltage calibration 
// values: Raw ADC data acquired at temperature of 30 degC with VDDA = 3.3V 
// during the manufacturing process
#define VREFINT_CALIBRATION_ADDRESS		(uint16_t *)(0x1FFFF7BA)

// Number of microcontroller pins that are configured to be ADC inputs
#ifndef NUM_ADC_CHANNELS
    #error "Please define the number pins configured as ADC inputs using NUM_ADC_CHANNELS!"
#endif

/** @brief Nominal voltage for VDDA, or ADC power supply */
#define VDDA_VOLTAGE (float32_t)(3.3f)

/** @brief Number of VREFINT channel. VREFINT is the internal voltage reference 
 *         that provides a stable (bandgap) voltage output for the ADC. */
#define NUM_VREFINT_CHANNEL 1

/**
 * @brief  Initialize ADC in DMA mode and the correct max ADC value
 * @param  hadc ADC handle
 * @param  vrefint_regular_rank The Regular Rank of VREFINT channel configured
 *         in STM32CubeMX (Starts at 1)
 */
void Io_SharedAdc_StartAdcInDmaMode(
    ADC_HandleTypeDef *hadc,
    uint32_t           vrefint_regular_rank);

/**
 * @brief  Get the appropriate maximum ADC value based on the ADC resolution
 * @return Maximum ADC value in bits
 */
uint32_t Io_SharedAdc_GetAdcMaxValue(void);

/**
 * @brief  Get the array of ADC readings transferred over DMA request
 * @return Pointer to the array of ADC readings
 */
const uint32_t *Io_SharedAdc_GetAdcValues(void);

/**
 * @brief  The VDDA power supply voltage applied to the microcontroller may be
 *         subject to variation or not precisely known. The embedded internal
 *         voltage reference (VREFINT) and its calibration data acquired by the
 *         ADC during the manufacturing process at VDDA = 3.3 V can be used to
 *         evaluate the actual VDDA voltage level.
 * @return Actual VDDA voltage
 */
float32_t Io_SharedAdc_GetActualVdda(void);

/**
 * @brief  Convert the ADC value to a voltage channel using the actual VDDA
 *         voltage level
 * @param  regular_rank The Regular Rank for the selected ADC channel as
 *         configured in STM32CubeMXa (Starts at 1)
 * @return The voltage at the ADC channel
 */
float32_t Io_SharedAdc_GetAdcVoltage(uint32_t regular_rank);

