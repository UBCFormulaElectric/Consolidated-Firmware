/**
 * @file  SharedAdc.h
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

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
// Number of microcontroller pins that are configured to be ADC inputs
#ifdef PDM 
    #define NUM_ADC_CHANNELS (uint32_t)(8)
#elif FSM
    // TODO: Correct this
    #define NUM_ADC_CHANNELS (uint32_t)(8)
#elif BMS
    // TODO: Correct this
    #define NUM_ADC_CHANNELS (uint32_t)(8)
#elif DCM
    // TODO: Correct this
    #define NUM_ADC_CHANNELS (uint32_t)(8)
#else
    #error "No valid PCB name selected"
#endif

/** @brief Nominal voltage for VDDA, or ADC power supply */
#define VDDA_VOLTAGE (float32_t)(3.3f)

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
extern uint32_t adc_max_value;
extern uint32_t adc_readings[NUM_ADC_CHANNELS];

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/**
 * @brief  Initialize ADC in DMA mode and the correct max ADC value
 * @param  hadc ADC handle
 * @param  data The destination buffer address
 * @param  length The length of data to be transferred from ADC peripheral to memory.
 */
void SharedAdc_StartAdcInDmaMode(ADC_HandleTypeDef *hadc, uint32_t *data, uint32_t length);

#endif /* SHARED_ADC_H */
