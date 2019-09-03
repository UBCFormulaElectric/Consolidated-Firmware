/**
 * @brief Shared ADC library
 */
#ifndef SHARED_ADC_H
#define SHARED_ADC_H

/******************************************************************************
 * Includes
 ******************************************************************************/
// Check for STM32 microcontroller family
#ifdef STM32F302x8
// Used in DCM 2017, BMS 2017, and PDM 2018
#include "stm32f3xx_hal.h"
#elif STM32F042x6
// Used in FSM 2017 (Shared ADC Library hasn't been tested on F0 yet)
#include "stm32f0xx_hal.h"
#else
#error \
    "No valid architecture selected - unable to determine what HAL library to use"
#endif

#include "arm_math.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

// Memory address where you can find the internal reference voltage calibration 
// values: Raw ADC data acquired at temperature of 30 degC with VDDA = 3.3V 
// during the manufacturing process
#ifdef STM32F302x8
#define VREFINT_CALIBRATION_ADDRESS		(uint16_t *)(0x1FFFF7BA)
#elif STM32F042x6
#else
#define VREFINT_CALIBRATION_ADDRESS		(uint16_t *)(0x1FFFF7BA)
#error
    "No valid architecture selected - unable to determine what HAL library to use"
#endif


// Number of microcontroller pins that are configured to be ADC inputs
#ifdef PDM
    #define NUM_ADC_CHANNELS (uint32_t)(9)
#elif FSM
    // TODO (#309): Correct this
    #define NUM_ADC_CHANNELS (uint32_t)(8)
#elif BMS
    // TODO (#309): Correct this
    #define NUM_ADC_CHANNELS (uint32_t)(8)
#elif DCM
    // TODO (#309): Correct this
    #define NUM_ADC_CHANNELS (uint32_t)(8)
#else
    #error "No valid PCB name selected"
#endif

/** @brief Nominal voltage for VDDA, or ADC power supply */
#define VDDA_VOLTAGE (float32_t)(3.3f)

/** @brief Number of VREFINT channel. VREFINT is the internal voltage reference 
 *         that provides a stable (bandgap) voltage output for the ADC. */
#define NUM_VREFINT_CHANNEL 1

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/**
 * @brief  Initialize ADC in DMA mode and the correct max ADC value
 * @param  hadc ADC handle
 * @param  vrefint_regular_rank The Regular Rank of VREFINT channel configured
 *         in STM32CubeMX (Starts at 1)
 */
void SharedAdc_StartAdcInDmaMode(
    ADC_HandleTypeDef *hadc,
    uint32_t           vrefint_regular_rank);

/**
 * @brief  Get the appropriate maximum ADC value based on the ADC resolution
 * @return Maximum ADC value in bits
 */
uint32_t SharedAdc_GetAdcMaxValue(void);

/**
 * @brief  Get the array of ADC readings transferred over DMA request
 * @return Pointer to the array of ADC readings
 */
const uint32_t *SharedAdc_GetAdcValues(void);

/**
 * @brief  The VDDA power supply voltage applied to the microcontroller may be
 *         subject to variation or not precisely known. The embedded internal
 *         voltage reference (VREFINT) and its calibration data acquired by the
 *         ADC during the manufacturing process at VDDA = 3.3 V can be used to
 *         evaluate the actual VDDA voltage level.
 * @return Actual VDDA voltage
 */
float32_t SharedAdc_GetActualVdda(void);

/**
 * @brief  Convert the ADC value to a voltage channel using the actual VDDA
 *         voltage level
 * @param  regular_rank The Regular Rank for the selected ADC channel as
 *         configured in STM32CubeMXa (Starts at 1)
 * @return The voltage at the ADC channel
 */
float32_t SharedAdc_GetAdcVoltage(uint32_t regular_rank);

#endif /* SHARED_ADC_H */
