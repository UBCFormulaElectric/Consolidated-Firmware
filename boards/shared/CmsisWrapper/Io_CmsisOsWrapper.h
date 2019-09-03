/**
 * @brief Wrappers for functions in cmsis_os.h
 */
#ifndef IO_CMSIS_OS_WRAPPER_H
#define IO_CMSIS_OS_WRAPPER_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "cmsis_os.h"

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
 * @brief  Delay a task until a specified time
 * @param   PreviousWakeTime   Pointer to a variable that holds the time at
 * which the task was last unblocked. PreviousWakeTime must be initialised with
 * the current time prior to its first use (PreviousWakeTime = osKernelSysTick()
 * )
 * @param   millisec    time delay value
 * @retval  status code that indicates the execution status of the function.
 */
osStatus Io_CmsisOsWrapper_osDelayUntilMs(
    uint32_t *PreviousWakeTime,
    uint32_t  millisec);

#endif // IO_CMSIS_OS_WRAPPER_H
