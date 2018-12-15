/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <string.h>
#include "SharedCan.h"

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
static CanTxMsgQueueItem_Struct can_tx_msg_fifo[CAN_TX_MSG_FIFO_SIZE];
static volatile uint8_t         tail = 0;
static volatile uint8_t         head = 0;

// mask_filters[] are initialized at compile-time so we don't
// need to waste resources during run-time to configure their values
#ifdef PDM
static CanMaskFilterConfig_Struct mask_filters[2] = {
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_DCM, MASKMODE_16BIT_MASK_DCM),
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED)};
#elif FSM
static CanMaskFilterConfig_Struct mask_filters[] = {
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED)};
#elif BMS
static CanMaskFilterConfig_Struct mask_filters[] = {
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_DCM, MASKMODE_16BIT_MASK_DCM),
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_FSM, MASKMODE_16BIT_MASK_FSM),
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_PDM, MASKMODE_16BIT_MASK_PDM),
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED)};
#elif DCM
static CanMaskFilterConfig_Struct mask_filters[] = {
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_FSM, MASKMODE_16BIT_MASK_FSM),
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED),
    INIT_MASK_FILTER(
        MASKMODE_16BIT_ID_BAMOCAR_TX,
        MASKMODE_16BIT_MASK_BAMOCAR_TX),
    INIT_MASK_FILTER(
        MASKMODE_16BIT_ID_BAMOCAR_RX,
        MASKMODE_16BIT_MASK_BAMOCAR_RX)};
#else
#error "No valid PCB selected - unable to determine what mask filters to use"
#endif

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/
/**
 * @brief  Transmit CAN message and remove it from the CAN queue
 * @return FIFO_IS_EMPTY: Failed dequeue due to empty queue
 *         FIFO_SUCCESS: Successful dequeue
 */
static Fifo_Status_Enum SharedCan_DequeueCanTxMessageFifo(void);

/**
 * @brief  Add CAN message overflow to CAN queue
 * @param  can_msg: Pointer to CAN message to be queued
 * @return FIFO_IS_FULL: Failed enqueue due to full queue
 *         FIFO_SUCCESS: Successful enqueue
 */
static Fifo_Status_Enum
    SharedCan_EnqueueCanTxMessageFifo(CanTxMsgQueueItem_Struct *can_msg);

/**
 * @brief  Clear the CAN queue
 */
static void SharedCan_ClearCanTxMessageFifo(void);

/**
 * @brief  Check if the CAN queue is full
 * @return false: CAN queue is not full
 *         true: CAN queue is full
 */
static bool SharedCan_CanTxMessageFifoIsFull(void);

/**
 * @brief  Check if the CAN queue is empty
 * @return false: CAN queue is not empty
 *         true: CAN queue is empty
 */
static bool SharedCan_CanTxMessageFifoIsEmpty(void);

/**
 * @brief  Get the number of messages saved in the CAN queue
 * @return Number of messages in the queue
 */
static uint32_t SharedCan_GetNumberOfItemsInCanTxMessageFifo(void);

/**
 * @brief  Initialize one or more CAN filters using 16-bit Filter Scale and
 *         Identifier Mask Mode (FSCx = 0, FBMx = 0)
 * @return ERROR: One or more filters didn't initialize properly
 *         SUCCESS: All filters initialized with no errors
 */
static ErrorStatus SharedCan_InitializeFilters(void);

/**
 * @brief  Shared callback function for every transmission mailbox
 *         (STM32F302x8's bxCAN peripheral has three - mailbox 0, 1, and 2).
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 */
static void Can_TxCommonCallback(CAN_HandleTypeDef *hcan);

/**
 * @brief  Send the overflow count for transmit FIFO over CAN. Note that this
 *         destroys one CAN message already enqueued.
 * @param  overflow_count Number of overflows occured thus far
 */
static void SharedCan_EnqueueFifoOverflowError(void);

/**
 * @brief Broadcast a CAN message to indicate that the system has rebooted
 */
static void SharedCan_BroadcastSystemReboot(void);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/
static Fifo_Status_Enum SharedCan_DequeueCanTxMessageFifo(void)
{
    if (!SharedCan_CanTxMessageFifoIsEmpty())
    {
        // Transmit one CAN message in queue
        SharedCan_TransmitDataCan(
            can_tx_msg_fifo[tail].std_id, can_tx_msg_fifo[tail].dlc,
            can_tx_msg_fifo[tail].data);

        // Remove the transmitted CAN message from queue
        memset(&can_tx_msg_fifo[tail], 0, sizeof(can_tx_msg_fifo[tail]));

        // Increment tail and make sure it wraps around to 0
        tail++;
        if (tail >= CAN_TX_MSG_FIFO_SIZE)
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

static Fifo_Status_Enum
    SharedCan_EnqueueCanTxMessageFifo(CanTxMsgQueueItem_Struct *can_msg)
{
    if (!SharedCan_CanTxMessageFifoIsFull())
    {
        // Add CAN message to queue
        can_tx_msg_fifo[head] = *can_msg;

        // Increment head and make sure it wraps around to 0
        head++;
        if (head >= CAN_TX_MSG_FIFO_SIZE)
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

static bool SharedCan_CanTxMessageFifoIsFull(void)
{
    return ((head + 1) % CAN_TX_MSG_FIFO_SIZE) == tail;
}

static bool SharedCan_CanTxMessageFifoIsEmpty(void)
{
    return head == tail;
}

static uint32_t SharedCan_GetNumberOfItemsInCanTxMessageFifo(void)
{
    uint32_t MessageCount;

    if (head >= tail)
    {
        MessageCount = head - tail;
    }
    else
    {
        MessageCount = CAN_TX_MSG_FIFO_SIZE - tail + head;
    }
    return MessageCount;
}

static ErrorStatus SharedCan_InitializeFilters(void)
{
    static uint32_t filter_bank = 0;
    static uint32_t fifo        = CAN_FILTER_FIFO0;
    uint32_t num_of_filters = sizeof(mask_filters) / sizeof(mask_filters[0]);
    uint32_t is_odd         = num_of_filters % 2;

    CAN_FilterTypeDef can_filter;
    can_filter.FilterMode       = CAN_FILTERMODE_IDMASK;
    can_filter.FilterScale      = CAN_FILTERSCALE_16BIT;
    can_filter.FilterActivation = CAN_FILTER_ENABLE;

    // Initialize two 16-bit filters for each filter bank
    for (uint32_t i = 0; i < num_of_filters / 2; i++)
    {
        // Configure filter settings
        can_filter.FilterIdLow          = mask_filters[i].id;
        can_filter.FilterMaskIdLow      = mask_filters[i].mask;
        can_filter.FilterIdHigh         = mask_filters[i + 1].id;
        can_filter.FilterMaskIdHigh     = mask_filters[i + 1].mask;
        can_filter.FilterFIFOAssignment = fifo;
        can_filter.FilterBank           = filter_bank;

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
        uint32_t last_filter_index      = num_of_filters - 1;
        can_filter.FilterIdLow          = mask_filters[last_filter_index].id;
        can_filter.FilterMaskIdLow      = mask_filters[last_filter_index].mask;
        can_filter.FilterIdHigh         = mask_filters[last_filter_index].id;
        can_filter.FilterMaskIdHigh     = mask_filters[last_filter_index].mask;
        can_filter.FilterFIFOAssignment = fifo;
        can_filter.FilterBank           = filter_bank;

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
    SharedCan_DequeueCanTxMessageFifo();
}

static void SharedCan_EnqueueFifoOverflowError(void)
{
    static uint32_t overflow_count = 0;
    overflow_count++;

    // Replace the next CAN message in queue with the overflow count in a
    // destructive manner
    can_tx_msg_fifo[tail].std_id = CAN_TX_FIFO_OVERFLOW_STDID;
    can_tx_msg_fifo[tail].dlc    = CAN_TX_FIFO_OVERFLOW_DLC;
    memcpy(
        &can_tx_msg_fifo[tail].data, &overflow_count, CAN_TX_FIFO_OVERFLOW_DLC);
}
static void SharedCan_BroadcastSystemReboot(void)
{
    uint8_t data[CAN_PAYLOAD_BYTE_SIZE] = {0};
    SharedCan_TransmitDataCan(PCB_STARTUP_STDID, PCB_STARTUP_DLC, &data[0]);
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void SharedCan_TransmitDataCan(
    CanStandardId_Enum     std_id,
    CanDataLengthCode_Enum dlc,
    uint8_t *              data)
{
    uint32_t mailbox =
        0; // Indicates the mailbox used for tranmission, not currently used
    CAN_TxHeaderTypeDef tx_header;

    tx_header.StdId = std_id;

    // The standard 11-bit CAN identifier is more than sufficent, so we disable
    // Extended CAN IDs by setting this field to zero.
    tx_header.ExtId = CAN_ExtID_NULL;

    // This field can be either Standard CAN or Extended CAN. See .ExtID to see
    // why we don't want Extended CAN
    tx_header.IDE = CAN_ID_STD;

    // This field can be either Data Frame or Remote Frame. For our
    // purpose, we only ever transmit Data Frames.
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = dlc;

    // Enabling this gives us a tick-based timestamp which we do not need and
    // would take up 2 bytes of the CAN payload. So we disable this setting.
    tx_header.TransmitGlobalTime = DISABLE;

    // If no mailbox is available or an error occured
    if (HAL_CAN_AddTxMessage(&hcan, &tx_header, data, &mailbox) != HAL_OK)
    {
        // Populate CAN TX message with CAN header and data
        CanTxMsgQueueItem_Struct tx_msg;
        tx_msg.std_id = std_id;
        tx_msg.dlc    = dlc;
        memcpy(&tx_msg.data, data, CAN_PAYLOAD_BYTE_SIZE);

        if (SharedCan_EnqueueCanTxMessageFifo(&tx_msg) == FIFO_IS_FULL)
        {
            SharedCan_EnqueueFifoOverflowError();
        }
    }
}

HAL_StatusTypeDef SharedCan_StartCanInInterruptMode(CAN_HandleTypeDef *hcan)
{
    HAL_StatusTypeDef status = HAL_OK;

    status |= SharedCan_InitializeFilters();

    status |= HAL_CAN_ActivateNotification(
        hcan, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING |
                  CAN_IT_RX_FIFO1_MSG_PENDING);

    status |= HAL_CAN_Start(hcan);

    SharedCan_BroadcastSystemReboot();

    return status;
}

__weak void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    /* NOTE: This function Should not be modified, when the callback is needed,
              the Can_RxCommonCallback could be implemented in the Can.c file */
}

void SharedCan_BroadcastHeartbeat(void)
{
    uint8_t data[PCB_HEARTBEAT_DLC] = {0};
    SharedCan_TransmitDataCan(PCB_HEARTBEAT_STDID, PCB_HEARTBEAT_DLC, &data[0]);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All receive mailbox interrupts shall be handled in the same way */
    Can_RxCommonCallback(hcan, CAN_RX_FIFO0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All receive mailbox interrupts shall be handled in the same way */
    Can_RxCommonCallback(hcan, CAN_RX_FIFO1);
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All transmit mailbox interrupts shall be handled in the same way */
    Can_TxCommonCallback(hcan);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All transmit mailbox interrupts shall be handled in the same way */
    Can_TxCommonCallback(hcan);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All transmit mailbox interrupts shall be handled in the same way */
    Can_TxCommonCallback(hcan);
}
