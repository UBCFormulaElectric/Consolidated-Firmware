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
static IwdgInitializeState_Enum iwdg_initialize_state = IWDG_NOT_INITIALIZED;

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
    if (SharedWatchdog_GetIwdgInitializeState() == IWDG_INITIALIZED)
    {
        HAL_IWDG_Refresh(&hiwdg);
    }
}

void SharedWatchdog_SetIwdgInitializeState(IwdgInitializeState_Enum state)
{
    iwdg_initialize_state = state;
}

IwdgInitializeState_Enum SharedWatchdog_GetIwdgInitializeState(void)
{
    return iwdg_initialize_state;
}