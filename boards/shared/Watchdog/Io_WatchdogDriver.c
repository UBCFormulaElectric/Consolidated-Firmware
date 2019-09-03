/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Io_WatchdogDriver.h"

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
void Io_WatchdogDriver_RefreshIwdg(void)
{
    if (Io_WatchdogDriver_IsIwdgInitialized())
    {
        HAL_IWDG_Refresh(hiwdg_ptr);
    }
}

void Io_WatchdogDriver_SetIwdgInitialized(IWDG_HandleTypeDef *hiwdg)
{
    hiwdg_ptr        = hiwdg;
    iwdg_initialized = true;
}

bool Io_WatchdogDriver_IsIwdgInitialized(void)
{
    return iwdg_initialized;
}
