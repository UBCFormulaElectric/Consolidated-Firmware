/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Io_Logging.h"
#include "Io_HalCallback.h"
#include "stm32f3xx_hal.h"

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
static void WaitAndReset(uint32_t delay_ms)
{
    // Delay a little before shutdown so the debug message can be sent out
    HAL_Delay(delay_ms);

    // Trigger a software reboot
    HAL_NVIC_SystemReset();
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void Io_HalCallback_ErrorHandler(char *file, uint32_t line)
{
    // Print debug information
    Io_Logging_Printf(
        "Error_Handler() called at %s:%d. Resetting.\r\n", file, line);

    WaitAndReset(DELAY_BETWEEN_ERROR_AND_SYSTEM_RESET_MS);
}

void Io_HalCallback_AssertFailed(char *file, uint32_t line)
{
    // Print debug information
    Io_Logging_Printf(
        "Wrong parameter value used at %s:%d. Resetting.\r\n", file, line);

    WaitAndReset(DELAY_BETWEEN_ERROR_AND_SYSTEM_RESET_MS);
}
