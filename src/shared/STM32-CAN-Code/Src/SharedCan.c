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
static CanTxMsgQueueItem_Struct can_tx_msg_fifo[CAN_TX_MSG_FIFO_SIZE];
static volatile uint8_t tail = 0;
static volatile uint8_t head = 0;

// mask_filters[] and can_headers[] are initialized at compile-time so we don't
// need to waste resources during run-time to configure their values
// TODO: How to use static on mask_filters[]? Is it on the array or the struct elements?
#ifdef PDM
static CanMaskFilterConfig_Struct mask_filters[2] =
{
	INIT_MASK_FILTER(MASKMODE_16BIT_ID_DCM, MASKMODE_16BIT_MASK_DCM),
	INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED)
};
#elif FSM  
static CanMaskFilterConfig_Struct mask_filters[] =
{
	INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED)
};
#elif BMS
static CanMaskFilterConfig_Struct mask_filters[] =
{
	INIT_MASK_FILTER(MASKMODE_16BIT_ID_DCM, MASKMODE_16BIT_MASK_DCM),
	INIT_MASK_FILTER(MASKMODE_16BIT_ID_FSM, MASKMODE_16BIT_MASK_FSM),
	INIT_MASK_FILTER(MASKMODE_16BIT_ID_PDM, MASKMODE_16BIT_MASK_PDM),
	INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED)
};
#elif DCM
static CanMaskFilterConfig_Struct mask_filters[] =
{
	INIT_MASK_FILTER(MASKMODE_16BIT_ID_FSM, MASKMODE_16BIT_MASK_FSM),
	INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED),
	INIT_MASK_FILTER(MASKMODE_16BIT_ID_BAMOCAR, MASKMODE_16BIT_MASK_BAMOCAR)
};
#endif

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/
/**
 * @brief  Transmit CAN message and remove it from the CAN queue
 * @param  None
 * @return FIFO_IS_EMPTY: Failed dequeue due to empty queue
 *         FIFO_SUCCESS: Successful dequeue
 */
static Fifo_Status_Enum SharedCan_DequeueCanTxMessageFifo(void);

/**
 * @brief  Add CAN message overflow to CAN queue
 * @param  can_msg: Pointer of CAN message to be queued
 * @return FIFO_IS_FULL: Failed enqueue due to full queue
 *         FIFO_SUCCESS: Successful enqueue
 */
static Fifo_Status_Enum SharedCan_EnqueueCanTxMessageFifo(CanTxMsgQueueItem_Struct *can_msg);

/**
 * @brief  Clear the CAN queue
 * @param  None
 * @return None
 */
static void SharedCan_ClearCanTxMessageFifo(void);

/**
 * @brief  Check if the CAN queue is full
 * @param  None
 * @return 0: CAN queue is not full
 *         1: CAN queue is full
 */
static uint32_t SharedCan_CanTxMessageFifoIsFull(void);

/**
 * @brief  Check if the CAN queue is empty
 * @param  None
 * @return 0: CAN queue is not empty
 *         1: CAN queue is empty
 */
static uint32_t SharedCan_CanTxMessageFifoIsEmpty(void);

/**
 * @brief  Get the number of messages saved in the CAN queue
 * @param  None
 * @return Number of messages in the queue
 */
static uint32_t SharedCan_GetNumberOfItemsInCanTxMessageFifo(void);

/**
 * @brief  Initialize one or more CAN filters using 16-bit Filter Scale and
 *         Identifier Mask Mode (FSCx = 0, FBMx = 0)
 * @param  None
 * @return ERROR:
 *         SUCCESS:
 */
static ErrorStatus SharedCAN_InitializeFilters(void);

/**
 * @brief  Shared callback function for transmission mailbox 0, 1, and 2
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @return None
 */
static void Can_TxCommonCallback(CAN_HandleTypeDef *hcan);

/******************************************************************************
* Private Function Definitions 
*******************************************************************************/
static Fifo_Status_Enum SharedCan_DequeueCanTxMessageFifo(void)
{
    if(!SharedCan_CanTxMessageFifoIsEmpty())
    {
        // Transmit one CAN message in queue
        SharedCan_TransmitDataCan(can_tx_msg_fifo[tail].std_id, can_tx_msg_fifo[tail].dlc, can_tx_msg_fifo[tail].data);

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

static Fifo_Status_Enum SharedCan_EnqueueCanTxMessageFifo(CanTxMsgQueueItem_Struct* can_msg)
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

static void SharedCan_ClearCanTxMessageFifo(void)
{
    memset(can_tx_msg_fifo, 0, sizeof(can_tx_msg_fifo));
}

static uint32_t SharedCan_CanTxMessageFifoIsFull(void)
{
    return ((head + 1) % CAN_TX_MSG_FIFO_SIZE) == tail;
}

static uint32_t SharedCan_CanTxMessageFifoIsEmpty(void)
{
    return head == tail;
}

static uint32_t SharedCan_GetNumberOfItemsInCanTxMessageFifo(void)
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

static ErrorStatus SharedCAN_InitializeFilters(void)
{
    static uint32_t filter_bank = 0;
    static uint32_t fifo = CAN_FILTER_FIFO0;
    uint32_t num_of_filters = sizeof(mask_filters) / sizeof(mask_filters[0]);
    uint32_t is_odd = num_of_filters % 2;

    CAN_FilterTypeDef can_filter;
    can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter.FilterScale = CAN_FILTERSCALE_16BIT;
    can_filter.FilterActivation = CAN_FILTER_ENABLE;

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
    
        // Configure and initialize filter bank
        if (HAL_CAN_ConfigFilter(&hcan, &can_filter) != HAL_OK)
        {
            return ERROR;
        }
    }

    return SUCCESS;
}

static void Can_TxCommonCallback(CAN_HandleTypeDef *hcan)
{
    if (!SharedCan_CanTxMessageFifoIsEmpty())
    {
        SharedCan_DequeueCanTxMessageFifo();
    }
}
/******************************************************************************
* Function Definitions
*******************************************************************************/
void SharedCan_BroadcastHeartbeat(Pcb_Enum module)
{
    //TODO: Implement when CAN Header structure is finalized
    //uint8_t Data[8] = {Module};
    //SharedCan_TransmitDataCan(can_headers[HEARTBEAT], Data);
}

void SharedCan_TransmitDataCan(uint32_t std_id, uint32_t dlc, uint8_t *data)
{
    #ifdef DEBUG
    static uint8_t overflow_count; // Debug variable to keep track of overflow before a proper handling strategy is developed
    static uint8_t transmit_error_count; // Debug variable to keep track of transmit errors before a proper handling strategy is developed
    #endif
    uint32_t mailbox = 0; // Indicates the mailbox used for tranmission, not currently used

    CAN_TxHeaderTypeDef tx_header;
    tx_header.StdId = std_id;
    tx_header.ExtId = CAN_ExtID_NULL;
    tx_header.IDE = CAN_ID_STD;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = dlc;
    tx_header.TransmitGlobalTime = DISABLE;

    // If a mailbox is not available or other error occurs
    if(HAL_CAN_AddTxMessage(&hcan, &tx_header, data, &mailbox) != HAL_OK)
    {
        // Populate CAN TX message with CAN header and data
        CanTxMsgQueueItem_Struct tx_msg;
        tx_msg.std_id = std_id;
        tx_msg.dlc = dlc;
        memcpy(&tx_msg.data, data,CAN_PAYLOAD_BYTE_SIZE);

        if(SharedCan_EnqueueCanTxMessageFifo(&tx_msg) != FIFO_SUCCESS)
        {
            #ifdef DEBUG
            // TODO: Handle CAN queue overflow error
            overflow_count++;
            #endif
        }
        #ifdef DEBUG
        //If error was not caused by full mailboxes
        if(!(hcan.ErrorCode & HAL_CAN_ERROR_PARAM))
        {
            transmit_error_count++;
        }
        #endif
    }
}

HAL_StatusTypeDef SharedCan_StartCanInInterruptMode(CAN_HandleTypeDef *hcan)
{
    HAL_StatusTypeDef status = HAL_OK;

    status |= SharedCAN_InitializeFilters();

    status |= HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY |
    CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING);  

    status |= HAL_CAN_Start(hcan);

    return status;
}


__weak void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    /* NOTE : This function Should not be modified, when the callback is needed,
              the Can_RxCommonCallback could be implemented in the Can.c file */
} 

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    // Both receive mailbox interrupts shall be handled in the same way
    Can_RxCommonCallback(hcan, CAN_RX_FIFO0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    // Both receive mailbox interrupts shall be handled in the same way
    Can_RxCommonCallback(hcan, CAN_RX_FIFO1);
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    // All 3 transmit mailbox interrupts shall be handled in the same way
    Can_TxCommonCallback(hcan);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    // All 3 transmit mailbox interrupts shall be handled in the same way
    Can_TxCommonCallback(hcan);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    // All 3 transmit mailbox interrupts shall be handled in the same way
    Can_TxCommonCallback(hcan);
}
