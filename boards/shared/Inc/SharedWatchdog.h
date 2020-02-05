/**
 * @brief Helper functions to unify the watchdog timer interface across boards
 */
#ifndef SHARED_WATCHDOG
#define SHARED_WATCHDOG

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "stm32f3xx_hal.h"
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
void SharedWatchdog_RefreshIwdg(void);

/**
 * @brief Set the boolean flag to indicate that the IWDG handle is initialized
 *        (To be used at the end of MX_IWDG_Init). And save a copy of the
 *        pointer to the IWDG handle.
 * @param hiwdg Pointer to the independent watchdog timer handle
 */
void SharedWatchdog_SetIwdgInitialized(IWDG_HandleTypeDef *hiwdg);

/**
 * @brief  Check if the IWDG handle is initialized yet
 * @return The current IWDG initialization state
 */
bool SharedWatchdog_IsIwdgInitialized(void);

#endif /* SHARED_WATCHDOG */
