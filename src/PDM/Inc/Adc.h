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
// Element 0 to 4 of adc_readings[] are current sense values
#define ADC_READINGS_CURRENT_START_INDEX 0

// Element 5 to 7 of adc_readings[] are voltage sense values
#define ADC_READINGS_VOLTAGE_START_INDEX 5

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
extern ADC_HandleTypeDef  hadc1;
extern volatile float32_t converted_readings[];
extern volatile uint8_t
                e_fuse_fault_states[NUM_ADC_CHANNELS * NUM_READINGS_PER_ADC_DMA_TRANSFER];
extern uint32_t adc_readings[NUM_ADC_CHANNELS];

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

#endif /* ADC_H */
