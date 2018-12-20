/******************************************************************************
 * Includes
 ******************************************************************************/
#include "ErrorHandling.h"
#include "CANDefinitions.h"
#include "Gpio.h"
#include "SharedGpio.h"

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
 ******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/

/**
 * @brief TODO  (Issue #191): Complete this
 */
void ErrorHandling_HandleHeartbeatTimeout(void)
{
    // Handle BMS not sending heartbeats

    // Kill inverters
    SharedGpio_GPIO_WritePin(
        EFUSE_DEN_5_GPIO_Port, EFUSE_DEN_5_Pin, GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_LEFT_INVERTER_IN_GPIO_Port, EFUSE_LEFT_INVERTER_IN_Pin,
        GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_RIGHT_INVERTER_IN_GPIO_Port, EFUSE_RIGHT_INVERTER_IN_Pin,
        GPIO_PIN_RESET);

    // Kill AIR SHDN
    SharedGpio_GPIO_WritePin(
        EFUSE_AIR_SHDN_IN_GPIO_Port, EFUSE_AIR_SHDN_IN_Pin, GPIO_PIN_RESET);

    // Log error
    Can_BroadcastPdmErrors(MISSING_HEARTBEAT);
}

/**
 * @brief TODO  (Issue #191): Complete this
 */
void ErrorHandling_HandleError(PdmError_Enum error) {}
