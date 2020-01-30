/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stm32f3xx_hal.h>
#include <FreeRTOS.h>
#include <task.h>
#include "SharedLogging.h"
#include "SharedAssert.h"
#include "SharedMacros.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
/**
 * @brief Delay between detecting an error and performing a system reset. This
 *        value is more-or-less arbitrarily decided.
 */
#define DELAY_BETWEEN_ERROR_AND_SYSTEM_RESET_MS 500U

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/
/**
 * @brief Simple wrapped to delay some time before performing a system reset.
 * @param delay_ms Amount of time to wait before rebooting
 */
static void WaitAndReset(uint32_t delay_ms);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static void __attribute__((unused)) WaitAndReset(uint32_t delay_ms)
{
    // Delay a little before shutdown so the debug message can be sent out
    HAL_Delay(delay_ms);

    // Trigger a software reboot
    HAL_NVIC_SystemReset();
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/

void SharedAssert_AssertFailed(char *file_path, uint32_t line, char *expr)
{
    taskDISABLE_INTERRUPTS();

    char *_expr =
        (expr == NULL) ? "No valid assert expression was provided" : expr;

    // Print an assert failure message that is similar to what you would find on
    // an x86 machine
    SharedLogging_Printf(
        "%s:%d %s: Assertion `%s' failed\r\n", __BASENAME__(file_path), line,
        __func__, _expr);

    BREAK_IF_DEBUGGER_CONNECTED();

    for (;;)
    {
        // Trap here forever. It might be desirable in the future to use
        // WaitAndReset() to make the system more fault tolerant, but for now
        // our main priority is making it obvious when an assert has been hit.

        // Uncomment the following when appropriate!
        // WaitAndReset(DELAY_BETWEEN_ERROR_AND_SYSTEM_RESET_MS);
    }
}
