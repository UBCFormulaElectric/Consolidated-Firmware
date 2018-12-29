/**
 * @file  Adc.h
 * @brief Analog-to-Digital Library
 */
#ifndef ADC_H
#define ADC_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stm32f3xx_hal.h"
#include "CurrentSense.h"
#include "FaultHandling.h"
#include "arm_math.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on

/******************************************************************************
 * Global Variables
 ******************************************************************************/
extern volatile GPIO_PinState dsel_state;
extern ADC_HandleTypeDef      hadc1;
extern volatile float32_t     converted_readings[];
extern volatile uint8_t
    e_fuse_fault_states[NUM_ADC_CHANNELS * NUM_READINGS_PER_ADC_DMA_TRANSFER];
extern uint32_t adc_readings[NUM_ADC_CHANNELS];

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

#endif /* ADC_H */
