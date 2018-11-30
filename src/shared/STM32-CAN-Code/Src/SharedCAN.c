/******************************************************************************
* Includes
*******************************************************************************/
#include <string.h>
#include "SharedCan.h"

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
// TODO: understand what the effect of using static on array is
static CanTxMsg_Struct can_tx_msg_fifo[CAN_TX_MSG_FIFO_SIZE];
static volatile uint8_t tail = 0;
static volatile uint8_t head = 0;

// mask_filters[] and can_headers[] are initialized at compile-time so we don't
// need to waste resources during run-time to configure their values
// TODO: How to use static on mask_filters[]? Is it on the array or the struct elements?
#if defined(PDM)
{
    static CanMaskFilterConfig_Struct mask_filters[] =
    {
        INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_DCM, IDMASK_16BIT_FILTER_MASK_DCM),
        INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_SHARED, IDMASK_16BIT_FILTER_MASK_SHARED)
    };
}
// TODO: Initialize mask filters and CAN headers for other PCBs
#elif defined(FSM)
{    
    static CanMaskFilterConfig_Struct mask_filters[] =
    {
        INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_SHARED, IDMASK_16BIT_FILTER_MASK_SHARED)
    };
}
#elif defined(BMS)
{
    static CanMaskFilterConfig_Struct mask_filters[] =
    {
        INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_DCM, IDMASK_16BIT_FILTER_MASK_DCM),
        INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_FSM, IDMASK_16BIT_FILTER_MASK_FSM),
        INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_PDM, IDMASK_16BIT_FILTER_MASK_PDM),
        INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_SHARED, IDMASK_16BIT_FILTER_MASK_SHARED)
    };
}
#elif defined(DCM)
{
    static CanMaskFilterConfig_Struct mask_filters[] =
    {
        INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_FSM, IDMASK_16BIT_FILTER_MASK_FSM),
        INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_SHARED, IDMASK_16BIT_FILTER_MASK_SHARED),
        INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_BAMOCAR, IDMASK_16BIT_FILTER_MASK_BAMOCAR)
    };
}
#endif

// Every PCB will use the same can_headers[]
CAN_TxHeaderTypeDef can_headers[] =
{
    INIT_CAN_HEADER(0x00, 8),  // BMS_ERROR
    INIT_CAN_HEADER(0x01, 1),  // BMS_HEARTBEAT
    INIT_CAN_HEADER(0x02, 3),  // BMS_ACCUMULATOR_STATUS
    INIT_CAN_HEADER(0x03, 4),  // BMS_ACCUMULATOR_DISCHARGE_POWER_LIMIT
    INIT_CAN_HEADER(0x04, 4),  // BMS_ACCUMULATOR_REGEN_POWER_LIMIT
    INIT_CAN_HEADER(0x05, 4),  // BMS_ACCUMULATOR_SOC
    INIT_CAN_HEADER(0x06, 1),  // BMS_TURN_ON_ALL_FETS
    INIT_CAN_HEADER(0x07, 4),  // BMS_CELL_VOLTAGE
    INIT_CAN_HEADER(0x08, 5),  // BMS_BALANCE_MOSFET
    INIT_CAN_HEADER(0x09, 1),  // BMS_LATCH_RESET
    INIT_CAN_HEADER(0x0A, 1),  // BMS_MOTOR_REENABLE
    INIT_CAN_HEADER(0x0B, 6),  // BMS_AIR_SHUTDOWN_ERROR
    INIT_CAN_HEADER(0x20, 8),  // DCM_ERROR
    INIT_CAN_HEADER(0x21, 1),  // DCM_HEARTBEAT
    INIT_CAN_HEADER(0x22, 4),  // DCM_LEFT_SLIP_RATIO
    INIT_CAN_HEADER(0x23, 4),  // DCM_RIGHT_SLIP_RATIO
    INIT_CAN_HEADER(0x24, 4),  // DCM_LEFT_MOTOR_OUTPUT
    INIT_CAN_HEADER(0x25, 4),  // DCM_RIGHT_MOTOR_OUTPUT
    INIT_CAN_HEADER(0x26, 4),  // DCM_LEFT_SLIP_ERROR
    INIT_CAN_HEADER(0x27, 4),  // DCM_RIGHT_SLIP_ERROR
    INIT_CAN_HEADER(0x40, 8),  // FSM_ERROR
    INIT_CAN_HEADER(0x41, 1),  // FSM_HEARTBEAT
    INIT_CAN_HEADER(0x42, 4),  // FSM_BRAKE_APPS
    INIT_CAN_HEADER(0x43, 4),  // FSM_STEERING_ANGLE
    INIT_CAN_HEADER(0x44, 4),  // FSM_LEFT_WHEEL_SPEED
    INIT_CAN_HEADER(0x45, 4),  // FSM_RIGHT_WHEEL_SPEED
    INIT_CAN_HEADER(0x60, 8),  // PDM_ERROR
    INIT_CAN_HEADER(0x61, 1),  // PDM_HEARTBEAT
    INIT_CAN_HEADER(0x62, 1),  // PDM_STARTUP_STATUS
    INIT_CAN_HEADER(0x80, 6),  // MOTOR_SHUTDOWN_ERROR
    INIT_CAN_HEADER(0x190, 3), // LEFT_TORQUE_REQUEST_TX
    INIT_CAN_HEADER(0x191, 3), // RIGHT_TORQUE_REQUEST_TX
    INIT_CAN_HEADER(0x210, 3), // LEFT_TORQUE_REQUEST_RX
    INIT_CAN_HEADER(0x211, 3)  // RIGHT_TORQUE_REQUEST_RX
};

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/
/******************************************************************************
* Private Function Definitions 
*******************************************************************************/
/******************************************************************************
* Function Definitions
*******************************************************************************/
Fifo_Status_Enum SharedCan_DequeueCanTxMessageFifo(void)
{
    if(!SharedCan_CanTxMessageFifoIsEmpty())
    {
        // Transmit one CAN message in queue
        SharedCan_TransmitDataCAN(&can_tx_msg_fifo[tail].tx_header, can_tx_msg_fifo[tail].data);

        // Remove the transmitted CAN message from queue
        memset(&can_tx_msg_fifo[tail], 0, sizeof(can_tx_msg_fifo[tail]));

        // Increment tail and make sure it wraps around to 0
        tail++;
        if(tail == CAN_TX_MSG_FIFO_SIZE)
        {
            tail = 0;
        }
        return FIFO_SUCCESS;
    }
    else
    {
        return FIFO_IS_EMPTY;
    }

}

Fifo_Status_Enum SharedCan_EnqueueCanTxMessageFifo(CanTxMsg_Struct* can_msg)
{
    if(!SharedCan_CanTxMessageFifoIsFull())
    {
        // Add CAN message to queue
        can_tx_msg_fifo[head] = *can_msg;

        // Increment head and make sure it wraps around to 0
        head++;
        if(head == CAN_TX_MSG_FIFO_SIZE)
        {
            head = 0;
        }
        return FIFO_SUCCESS;
    }
    else
    {
        return FIFO_IS_FULL;
    }
}

void SharedCan_ClearCanTxMessageFifo(void)
{
    memset(can_tx_msg_fifo, 0, sizeof(can_tx_msg_fifo));
}

uint32_t SharedCan_CanTxMessageFifoIsFull(void)
{
    return ((head + 1) % CAN_TX_MSG_FIFO_SIZE) == tail;
}

uint32_t SharedCan_CanTxMessageFifoIsEmpty(void)
{
    return head == tail;
}

uint32_t SharedCan_GetNumberOfItemsInCanTxMessageFifo(void)
{
    uint32_t MessageCount;

    if(head >= tail)
    {
        MessageCount = head - tail;
    }
    else
    {
        MessageCount = CAN_TX_MSG_FIFO_SIZE - tail + head;
    }
    return MessageCount;
}

void SharedCan_BroadcastHeartbeat(Pcb_Enum module)
{
    //TODO: Implement when CAN Header structure is finalized
    //uint8_t Data[8] = {Module};
    //SharedCan_TransmitDataCAN(can_headers[HEARTBEAT], Data);
}

void SharedCan_TransmitDataCAN(CAN_TxHeaderTypeDef *tx_header, uint8_t * data)
{
    #ifdef DEBUG
    static uint8_t overflow_count; // Debug variable to keep track of overflow before a proper handling strategy is developed
    static uint8_t transmit_error_count; // Debug variable to keep track of transmit errors before a proper handling strategy is developed
    #endif
    uint32_t mailbox = 0; // Indicates the mailbox used for tranmission, not currently used

    // If a mailbox is not available or other error occurs
    if(HAL_CAN_AddTxMessage(&hcan, tx_header, data, &mailbox) != HAL_OK)
    {
        // Populate CAN TX message with CAN header and data
        CanTxMsg_Struct queue_msg;
        queueMsg.tx_header = *tx_header;
        memcpy(&queueMsg.Data, data,CAN_PAYLOAD_SIZE);

        if(SharedCan_EnqueueCanTxMessageFifo(&queue_msg) != FIFO_SUCCESS)
        {
            #ifdef DEBUG
            // TODO: Handle CAN queue overflow error
            overflow_count++;
            #endif
        }
        #ifdef DEBUG
        //If error was not caused by full mailboxes
        if((hcan.ErrorCode & HAL_CAN_ERROR_PARAM) != HAL_CAN_ERROR_PARAM)
        {
            transmit_error_count++;
        }
        #endif
    }
}

ErrorStatus SharedCAN_InitializeFilters(void)
{
    static uint32_t filter_bank = 0;
    static uint32_t fifo = CAN_FILTER_FIFO0;
    uint32_t num_of_filters = sizeof(mask_filters) / sizeof(mask_filters[0]);
    uint32_t is_odd = num_of_filters % 2;

    // TODO: is it ok to re-use can_filter for each filter configuration?
    CAN_FilterTypeDef can_filter;

    // Initialize two 16-bit filters for each filter bank
    for (uint32_t i = 0; i < num_of_filters / 2; i ++)
    {
        // Configure filter settings
        can_filter.FilterIdLow = mask_filters[i].id;
        can_filter.FilterMaskIdLow = mask_filters[i].mask;
        can_filter.FilterIdHigh = mask_filters[i + 1].id;
        can_filter.FilterMaskIdHigh = mask_filters[i + 1].mask;
        can_filter.FilterFIFOAssignment = fifo;
        can_filter.FilterBank = filter_bank;
        // TODO: Consider moving the 3 lines below outside of the for-loop
        can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
        can_filter.FilterScale = CAN_FILTERSCALE_16BIT;
        can_filter.FilterActivation = CAN_FILTER_ENABLE;

        // Alternate between the two FIFOs
        fifo = !fifo;

        // Update filter bank for next iteration
        filter_bank = filter_bank + 1;

        // Configure and initialize filter bank
        if (HAL_CAN_ConfigFilter(&hcan, &can_filter) != HAL_OK)
        {
            return ERROR;
        }
    }

    // For the odd-one-out filter, initialize two identical 16-bit filters
    // because each filter bank requires two 16-bit filters
    if (is_odd)
    {
        // Configure filter settings
        uint32_t last_filter_index = num_of_filters - 1;
        can_filter.FilterIdLow = mask_filters[last_filter_index].id;
        can_filter.FilterMaskIdLow = mask_filters[last_filter_index].mask;
        can_filter.FilterIdHigh = mask_filters[last_filter_index].id;
        can_filter.FilterMaskIdHigh = mask_filters[last_filter_index].mask;
        can_filter.FilterFIFOAssignment = fifo;
        can_filter.FilterBank = filter_bank;
        can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
        can_filter.FilterScale = CAN_FILTERSCALE_16BIT;
        can_filter.FilterActivation = CAN_FILTER_ENABLE;

        // Configure and initialize filter bank
        if (HAL_CAN_ConfigFilter(&hcan, &can_filter) != HAL_OK)
        {
            return ERROR;
        }
    }

    return SUCCESS;
}

__weak void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    /* NOTE : This function Should not be modified, when the callback is needed,
              the Can_RxCommonCallback could be implemented in the Can.c file */
} 

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    Can_RxCommonCallback(hcan, CAN_RX_FIFO0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    Can_RxCommonCallback(hcan, CAN_RX_FIFO1);
}

__weak void Can_TxCommonCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE : This function Should not be modified, when the callback is needed,
              the Can_RxCommonCallback could be implemented in the Can.c file */
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    Can_TxCommonCallback(hcan);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    Can_TxCommonCallback(hcan);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    Can_TxCommonCallback(hcan);
}