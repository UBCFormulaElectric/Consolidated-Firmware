/**
 * @file  SharedGpio.h
 * @brief Shared library for wrapping HAL GPIO library
 */
//TODO: Update header guard below to an unique identifier in this project
#ifndef SHARED_GPIO_H
#define SHARED_GPIO_H

/******************************************************************************
 * Includes
 ******************************************************************************/
// Check for STM32 microcontroller family
#ifdef STM32F302x8
// Used in DCM 2017, BMS 2017, and PDM 2018
#include "stm32f3xx_hal.h"
#elif STM32F042x6
// Used in FSM 2017 (Shared CAN Library doesn't yet support this)
#include "stm32f0xx_hal.h"
#else
#error "No valid architecture selected - unable to determine what HAL library to use"
#endif

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off

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
void SharedGpio_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t PinState);

#endif /* SHARED_GPIO_H */
