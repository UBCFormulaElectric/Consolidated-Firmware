/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Io/Io_SharedCmsisOs.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/

/******************************************************************************
 * Private Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
/**
 * @brief  Delay a task until a specified time
 * @param   PreviousWakeTime   Pointer to a variable that holds the time at
 * which the task was last unblocked. PreviousWakeTime must be initialised with
 * the current time prior to its first use (PreviousWakeTime = osKernelSysTick()
 * )
 * @param   millisec    time delay value
 * @retval  status code that indicates the execution status of the function.
 */
osStatus Io_SharedCmsisOs_osDelayUntilMs(
    uint32_t *PreviousWakeTime,
    uint32_t  millisec)
{
    return osDelayUntil(PreviousWakeTime, millisec);
}
