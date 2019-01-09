/**
 * @file  SharedWatchdog.h
 * @brief Helper functions to unify the watchdog timer interface across boards
 */
#ifndef SHARED_WATCHDOG
#define SHARED_WATCHDOG

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
extern IWDG_HandleTypeDef hiwdg;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief A wrapper for HAL_IWDG_Refresh that only refreshes the IWDG if the
 *        handle has already been initialized
 */
void SharedWatchdog_RefreshIwdg(void);

/**
 * @brief Set the boolean flag to indicate that the IWDG handle is initialized
 *        (To be used at the end of MX_IWDG_Init)
 */
void SharedWatchdog_SetIwdgInitialized(void);

/**
 * @brief  Check if the IWDG handle is initialized yet
 * @return The current IWDG initialization state
 */
bool SharedWatchdog_IsIwdgInitialized(void);

#endif /* SHARED_WATCHDOG */
