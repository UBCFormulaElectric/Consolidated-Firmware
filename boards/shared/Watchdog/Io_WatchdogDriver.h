/**
 * @brief Watchdog timer driver built on top of STM32 HAL library
 */
#ifndef IO_WATCHDOG_DRIVER_H
#define IO_WATCHDOG_DRIVER_H

/******************************************************************************
 * Includes
 ******************************************************************************/
// Check for STM32 microcontroller family
#ifdef STM32F302x8
// Used in DCM 2017, BMS 2017, and PDM 2018
#include "stm32f3xx_hal.h"
#elif STM32F042x6
// Used in FSM 2017
#include "stm32f0xx_hal.h"
#else
#error \
    "No valid architecture selected - unable to determine what HAL library to use"
#endif

#include "stdbool.h"

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
/**
 * @brief A wrapper for HAL_IWDG_Refresh that only refreshes the IWDG if the
 *        handle has already been initialized
 */
void Io_WatchdogDriver_RefreshIwdg(void);

/**
 * @brief Set the boolean flag to indicate that the IWDG handle is initialized
 *        (To be used at the end of MX_IWDG_Init). And save a copy of the
 *        pointer to the IWDG handle.
 * @param hiwdg Pointer to the independent watchdog timer handle
 */
void Io_WatchdogDriver_SetIwdgInitialized(IWDG_HandleTypeDef *hiwdg);

/**
 * @brief  Check if the IWDG handle is initialized yet
 * @return The current IWDG initialization state
 */
bool Io_WatchdogDriver_IsIwdgInitialized(void);

#endif /* IO_WATCHDOG_DRIVER_H */
