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
        EFUSE_INVERTER_DEN_PORT, EFUSE_INVERTER_DEN_PIN, GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_LEFT_INVERTER_IN_PORT, EFUSE_LEFT_INVERTER_IN_PIN,
        GPIO_PIN_RESET);
    SharedGpio_GPIO_WritePin(
        EFUSE_RIGHT_INVERTER_IN_PORT, EFUSE_RIGHT_INVERTER_IN_PIN,
        GPIO_PIN_RESET);

    // Kill AIR SHDN
    SharedGpio_GPIO_WritePin(
        EFUSE_AIR_SHDN_IN_PORT, EFUSE_AIR_SHDN_IN_PIN, GPIO_PIN_RESET);

    // Log error
    TransmitCANError(
        PDM_ERROR, Power_Distribution_Module, MISSING_HEARTBEAT, 0);
}

/**
 * @brief TODO  (Issue #191): Complete this
 */
void ErrorHandling_HandleError(PdmError_Enum error) {}
