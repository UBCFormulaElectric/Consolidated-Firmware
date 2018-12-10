/******************************************************************************
* Includes
*******************************************************************************/
#include "Can.h"

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
#ifndef DEBUG

// Heartbeat Setup
volatile uint16_t HeartbeatCount[PCB_COUNT] = {0};
const int HEARTBEAT_TICK_PERIOD             = 1000; // Period in ms
const int HEARTBEAT_BROADCAST_PERIOD        = 300;  // Period in ms

#endif

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/
static void Can_HeartbeatCallback(uint8_t *data);
/******************************************************************************
* Private Function Definitions
*******************************************************************************/
static void Can_HeartbeatCallback(uint8_t *data)
{
    /* TODO: Need bit mask for DLC = 4 */
    Pcb_Enum pcb = data[0];
    HeartbeatCount[pcb]++;
}
/******************************************************************************
* Function Definitions
*******************************************************************************/
void Can_BroadcastErrors(PdmError_Enum errors)
{
    uint32_t data = 1U << errors;
    SharedCan_TransmitDataCan(PDM_ERROR_STDID, PDM_ERROR_DLC, (uint8_t*)&data);
}

void Can_RxCommonCallback(CAN_HandleTypeDef* hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;

    HAL_CAN_GetRxMessage(hcan, rx_fifo, &rx_msg.rx_header, &rx_msg.data[0]);

    switch(rx_msg.rx_header.StdId)
    {
    #ifndef DEBUG
        case PDM_HEARTBEAT_STDID:
            Can_HeartbeatCallback(&rx_msg.data[0]);
            break;
    #endif
        default:
            break;
    }
}
