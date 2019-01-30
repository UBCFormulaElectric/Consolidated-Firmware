/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Can.h"
#include "SharedHeartbeat.h"
#include "CanMsgs.h"

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
void Can_BroadcastAirShutdownError(void)
{
    struct CanMsgs_pdm_air_shutdown_t shutdown_msg = { 0 };
    SHAREDCAN_SEND_CAN_MSG(pdm_air_shutdown, &shutdown_msg);
}

void Can_BroadcastMotorShutdownError(void)
{
    struct CanMsgs_pdm_motor_shutdown_t shutdown_msg = { 0 };
    SHAREDCAN_SEND_CAN_MSG(pdm_motor_shutdown, &shutdown_msg);
}

void Can_BmsStartupCallback(struct CanMsgs_bms_startup_id_t bms_startup_msg)
{
    // TODO: Placeholder
}

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;

    HAL_CAN_GetRxMessage(hcan, rx_fifo, &rx_msg.rx_header, &rx_msg.data[0]);

    // Check for a received heartbeat
    Heartbeat_HandleHeartbeatReception(rx_msg.rx_header.StdId);

    SHAREDCAN_CAN_MSG_TO_CALLBACK_MAPPING(rx_msg.rx_header.StdId, rx_msg.data)
    {
        SHAREDCAN_IF_STDID_IS(bms_startup_id, Can_BmsStartupCallback);
    }
}
