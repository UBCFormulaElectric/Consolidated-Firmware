/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Can.h"

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
    // TODO (#Issue 217): Is it ok for payload to be empty?
    uint8_t data[CAN_PAYLOAD_BYTE_SIZE] = { 0 };
    SharedCan_TransmitDataCan(
        BMS_AIR_SHUTDOWN_ERROR_STDID, BMS_AIR_SHUTDOWN_ERROR_DLC, &data[0]);
}

void Can_BroadcastMotorShutdownError(void)
{
    // TODO (#Issue 217): Is it ok for payload to be empty?
    uint8_t data[CAN_PAYLOAD_BYTE_SIZE] = { 0 };
    SharedCan_TransmitDataCan(
        SHARED_MOTOR_SHUTDOWN_ERROR_STDID, SHARED_MOTOR_SHUTDOWN_ERROR_DLC,
        &data[0]);
}

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;

    HAL_CAN_GetRxMessage(hcan, rx_fifo, &rx_msg.rx_header, &rx_msg.data[0]);

    switch (rx_msg.rx_header.StdId)
    {
        default:
            break;
    }
}
