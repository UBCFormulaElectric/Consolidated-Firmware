/**
 *   @file       FaultHandling.h
 *   @brief      PDM 2018 Code
 *   @author     UBC Formula Electric
**/

#ifndef FAULTHANDLING_H
#define FAULTHANDLING_H

#include "stm32f3xx_hal.h"
#include "CurrentSense.h"

extern __IO GPIO_PinState DSEL_State;

/**
 *   @brief      Checks if any current or voltage readings exceed their
 *               respective limits and executes output-specific fault
 *               handling behaviour.
 *
 *  @param		fault_states       Array with NumReadings*ChannelCount
 *                                 elements which tracks outputs that
 *                                 need to be renabled or are
 *                                 permanently faulted
 *  @param		converted_readings Array with NumReadings*ChannelCount
 *                                 elements that tracks converted
 *                                 current/voltage readings from ADC
 *                                 counts to A or V
 *  @return     None
**/
void FaultHandling_Handler(
    __IO uint8_t *fault_states,
    __IO float *  converted_readings);

/**
 *  @brief      Renable any E-Fuses that have faulted but have not exceeded
 *              their max number of retries
 *
 *  @param		fault_states Array with 2*ChannelCount elements that tracks
 *                           outputs which need to be renabled or are
 *                           permanently off (error state)
 *  @return     None
**/
void FaultHandling_RetryEFuse(__IO uint8_t *fault_states);

#endif
