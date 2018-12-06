/******************************************************************************
* Includes
*******************************************************************************/
#include "ErrorHandling.h"
#include "CANDefinitions.h"
#include "Gpio.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

/******************************************************************************
* Private Function Definitions
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/

/**
 * @brief TODO: Complete this
 */
void ErrorHandling_HandleHeartbeatTimeout(void) {
    // Handle BMS not sending heartbeats

    // Kill inverters
    HAL_GPIO_WritePin(
    EFUSE_INVERTER_DEN_PORT, EFUSE_INVERTER_DEN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
    EFUSE_LEFT_INVERTER_IN_PORT, EFUSE_LEFT_INVERTER_IN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(
    EFUSE_RIGHT_INVERTER_IN_PORT, EFUSE_RIGHT_INVERTER_IN_PIN, GPIO_PIN_RESET);

    // Kill AIR SHDN
    HAL_GPIO_WritePin(
    EFUSE_AIR_SHDN_IN_PORT, EFUSE_AIR_SHDN_IN_PIN, GPIO_PIN_RESET);

    // Log error
    TransmitCANError(
    PDM_ERROR, Power_Distribution_Module, MISSING_HEARTBEAT, 0);
}

/**
 * @brief TODO: Complete this
 */
void ErrorHandling_HandleError(PdmError_Enum Error) {}
