/**
 * @file  Dma.h
 * @brief Direct Memory Access Library
 */
#ifndef DMA_H
#define DMA_H

/******************************************************************************
* Includes
*******************************************************************************/
#include "stm32f3xx_hal.h"
#include "CurrentSense.h"
#include "FaultHandling.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
// clang-format off

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on

/******************************************************************************
* Global Variables
*******************************************************************************/
extern volatile GPIO_PinState dsel_state;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern volatile uint32_t adc_readings[];
extern volatile float converted_readings[];
extern volatile uint8_t e_fuse_fault_states[ADC_CHANNEL_COUNT * NUM_READINGS];

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
 * @brief Initialize DMA
 */
void DMA_Init(void);

/**
 * @brief TODO (Issue #191): Fill in information (I think this may be removed entirely)
 */
void DMA_TransferCompleteCallback(DMA_HandleTypeDef* _hdma);

#endif /* DMA_H */
