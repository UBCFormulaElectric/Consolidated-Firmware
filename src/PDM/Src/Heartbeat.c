/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedHeartbeat.h"
#include "SharedCan.h"
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
void Heartbeat_HandleHeartbeatTimeout(uint8_t heartbeats_received)
{
    // Handle BMS heartbeat timeout

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
    SHAREDCAN_SEND_CAN_MSG_WITH_SINGLE_BIT_SET(pdm_errors, missing_heartbeat);
}

void Heartbeat_HandleHeartbeatReception(uint32_t std_id)
{
    if (std_id == CANMSGS_bms_startup_FRAME_ID)
    {
        SharedHeartbeat_ReceiveHeartbeat(BMS_HEARTBEAT_ENCODING);
    }
}
