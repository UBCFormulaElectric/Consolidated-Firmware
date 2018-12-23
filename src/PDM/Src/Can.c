/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Can.h"

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
#ifndef DEBUG

// Heartbeat Setup
volatile uint16_t HeartbeatCount[PCB_COUNT]  = { 0 };
const int         HEARTBEAT_TICK_PERIOD      = 1000; // Period in ms
const int         HEARTBEAT_BROADCAST_PERIOD = 300;  // Period in ms

#endif

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/
static void Can_HeartbeatCallback(uint8_t *data);
/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static void Can_HeartbeatCallback(uint8_t *data)
{
    /* TODO: Need bit mask for DLC = 4 */
    Pcb_Enum pcb = data[0];
    HeartbeatCount[pcb]++;
}
/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void Can_BroadcastAirShutdownError(void)
{
    // TODO (#Issue 217): Is it ok for payload to be empty?
    uint8_t data[CAN_PAYLOAD_BYTE_SIZE] = {0};
    SharedCan_TransmitDataCan(
        BMS_AIR_SHUTDOWN_ERROR_STDID, BMS_AIR_SHUTDOWN_ERROR_DLC, &data[0]);
}

void Can_BroadcastMotorShutdownError(void)
{
    // TODO (#Issue 217): Is it ok for payload to be empty?
    uint8_t data[CAN_PAYLOAD_BYTE_SIZE] = {0};
    SharedCan_TransmitDataCan(
        SHARED_MOTOR_SHUTDOWN_ERROR_STDID, SHARED_MOTOR_SHUTDOWN_ERROR_DLC,
        &data[0]);
}

void Can_BroadcastPdmErrors(PdmError_Enum errors)
{
    // Error message is one-hot encoded according to PdmError_enum and thus
    // requires bit shifting
    uint32_t data = 1U << errors;
    SharedCan_TransmitDataCan(PDM_ERROR_STDID, PDM_ERROR_DLC, (uint8_t *)&data);
}

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;

    HAL_CAN_GetRxMessage(hcan, rx_fifo, &rx_msg.rx_header, &rx_msg.data[0]);

    switch (rx_msg.rx_header.StdId)
    {
/* Disable heartbeat callback in DEBUG mode because breakpoints will stop 
 * heartbeat broadcasting */
#ifndef DEBUG
        case PDM_HEARTBEAT_STDID:
            Can_HeartbeatCallback(&rx_msg.data[0]);
            break;
#endif
        default:
            break;
    }
}
