/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedWatchdog.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
// IWDG_HandleTypeDef hiwdg is not initialized on system boot-up
static bool iwdg_initialized = false;

// A copy of the pointer to hiwdg just for this translation unit
static IWDG_HandleTypeDef *hiwdg_ptr;

/******************************************************************************
 * Private Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void SharedWatchdog_RefreshIwdg(void)
{
    if (SharedWatchdog_IsIwdgInitialized())
    {
        HAL_IWDG_Refresh(hiwdg_ptr);
    }
}

void SharedWatchdog_SetIwdgInitialized(IWDG_HandleTypeDef *hiwdg)
{
    hiwdg_ptr        = hiwdg;
    iwdg_initialized = true;
}

bool SharedWatchdog_IsIwdgInitialized(void)
{
    return iwdg_initialized;
}
