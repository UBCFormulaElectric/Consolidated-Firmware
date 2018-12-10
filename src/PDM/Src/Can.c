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
volatile uint16_t HeartbeatCount[Systems_Count] = {0};
const int HEARTBEAT_TICK_PERIOD             = 1000; // Period in ms
const int HEARTBEAT_BROADCAST_PERIOD        = 300;  // Period in ms

#endif

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/

/******************************************************************************
* Private Function Definitions
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
void TransmitCANError(uint32_t Error_StandardID,
                      PCB_Enum Module,
                      uint8_t ErrorNumber,
                      uint32_t ErrorData) {
    uint64_t Msg =
    ((uint64_t) Module << 40) + ((uint64_t) ErrorNumber << 32) + ErrorData;
    uint8_t Data[8];

    memcpy(Data, &Msg, sizeof(Msg));
    // TODO (Issue #192): Change function to take in CAN_TxHeaderTypeDef instead of
    // Error_StandardID
    CAN_TxHeaderTypeDef TxErrorHeader;

    // TODO (Issue #192): Can be simplified
    switch (Error_StandardID) {
        case BMS_AIR_SHUTDOWN_ERROR:
            TxErrorHeader = can_headers[BMS_AIR_SHUTDOWN_ERROR];
            break;
        case MOTOR_SHUTDOWN_ERROR:
            TxErrorHeader = can_headers[MOTOR_SHUTDOWN_ERROR];
            break;
        case PDM_ERROR: TxErrorHeader = can_headers[PDM_ERROR]; break;
        default:
            // Note: Default error had hard coded values, should be changed to a
            // default header in future if needed
            TxErrorHeader.StdId              = Error_StandardID;
            TxErrorHeader.ExtId              = 0;
            TxErrorHeader.IDE                = CAN_ID_STD;
            TxErrorHeader.RTR                = CAN_RTR_DATA;
            TxErrorHeader.DLC                = 4;
            TxErrorHeader.TransmitGlobalTime = DISABLE;
            break;
    }

    // workaround until CANDatabase is updated with headers
    // SharedCAN_TransmitDataCAN(&TxErrorHeader, Data);
}

void InitCAN(void) {
    SharedCAN_InitializeHeaders();

    // Enable CAN interrupts
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY |
    CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING);

    // Enable CAN module
    HAL_CAN_Start(&hcan);
}

void CAN_RxCommonCallback(CAN_HandleTypeDef* hcan, uint32_t RxFifo) {
    CAN_RxHeaderTypeDef pHeader;
    uint8_t aData[CAN_PAYLOAD_SIZE];
    HAL_CAN_GetRxMessage(hcan, RxFifo, &pHeader, aData);
/*
    switch (pHeader->StdId) {
#ifndef DEBUG
        case Heartbeat_StandardID:
            // Process Heartbeat info here
            Board = (Module_Name) aData[0];

            // Case statement used in case data is outside of array bounds.
            // Special cases / behaviour can also be handled here.
            switch (Board) {
                case Battery_Management_System:
                    HeartbeatCount[Battery_Management_System]++;
                default:
                    // Log error
                    break;
            }
            break;
#endif
        default: break;
    }
    */
}

void CAN_TxCommonCallback(CAN_HandleTypeDef* hcan) {
    if (SharedCAN_DequeueCanTxMessageFifo() == FIFO_ERROR) {
        SharedCAN_ClearCanTxMessageFifo();
    }
}
