/******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "App_CanMsgsTx.h"
#include "SharedCan.h"
#include "main.h"
#include "SharedFreeRTOS.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
#define CAN_TX_MSG_FIFO_ITEM_SIZE sizeof(struct CanTxMsg)
#define CAN_TX_MSG_FIFO_LENGTH 20 // CAN FIFO length is arbitrary at the moment

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
/** @brief FIFO for buffering CAN TX messages */
static uint8_t
    can_tx_msg_fifo_storage[CAN_TX_MSG_FIFO_LENGTH * CAN_TX_MSG_FIFO_ITEM_SIZE];

static struct FreeRTOSStaticQueue can_tx_msg_fifo = {
    .storage = &can_tx_msg_fifo_storage[0],
};

// TODO (Issue: 243): Remove clang-format on/off once #243 is resolved
// clang-format off
// mask_filters[] are initialized at compile-time so we don't
// need to waste resources during run-time to configure their values
#ifdef PDM
#elif FSM
#elif BMS
static CanMaskFilterConfig_Struct mask_filters[4] =
{
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_DCM, MASKMODE_16BIT_MASK_DCM),
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_FSM, MASKMODE_16BIT_MASK_FSM),
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_PDM, MASKMODE_16BIT_MASK_PDM),
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED)
};
#elif DCM
#else
#error "No valid PCB selected - unable to determine what mask filters to use"
#endif
// clang-format on

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/
/**
 * @brief  Initialize one or more CAN filters using 16-bit Filter Scale and
 *         Identifier Mask Mode (FSCx = 0, FBMx = 0)
 * @param  filters Array of CAN filters.
 * @param  num_of_filters The number of CAN filters in the array.
 * @return ERROR: One or more filters didn't initialize properly
 *         SUCCESS: All filters initialized with no errors
 */
static ErrorStatus SharedCan_InitializeFilters(
    CanMaskFilterConfig_Struct filters[],
    uint32_t                   num_of_filters);

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
static ErrorStatus SharedCan_InitializeFilters(
    CanMaskFilterConfig_Struct filters[],
    uint32_t                   num_of_filters)
{
    static uint32_t filter_bank = 0;
    static uint32_t fifo        = CAN_FILTER_FIFO0;
    uint32_t        is_odd      = num_of_filters % 2;

    CAN_FilterTypeDef can_filter;
    can_filter.FilterMode       = CAN_FILTERMODE_IDMASK;
    can_filter.FilterScale      = CAN_FILTERSCALE_16BIT;
    can_filter.FilterActivation = CAN_FILTER_ENABLE;

    // Initialize two 16-bit filters for each filter bank
    for (uint32_t i = 0; i < num_of_filters / 2; i++)
    {
        // Configure filter settings
        can_filter.FilterIdLow          = filters[i].id;
        can_filter.FilterMaskIdLow      = filters[i].mask;
        can_filter.FilterIdHigh         = filters[i + 1].id;
        can_filter.FilterMaskIdHigh     = filters[i + 1].mask;
        can_filter.FilterFIFOAssignment = fifo;
#ifdef STM32F042x6
        can_filter.BankNumber   = filter_bank;
        can_filter.FilterNumber = i;
#else
        can_filter.FilterBank = filter_bank;
#endif

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
        can_filter.FilterIdLow          = filters[last_filter_index].id;
        can_filter.FilterMaskIdLow      = filters[last_filter_index].mask;
        can_filter.FilterIdHigh         = filters[last_filter_index].id;
        can_filter.FilterMaskIdHigh     = filters[last_filter_index].mask;
        can_filter.FilterFIFOAssignment = fifo;
#ifdef STM32F042x6
        can_filter.BankNumber = filter_bank;
#else
        can_filter.FilterBank = filter_bank;
#endif

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
    UNUSED(hcan);
    struct CanTxMsg message;
    BaseType_t      HigherPriorityTaskWoken = pdFALSE;
    if (xQueueReceiveFromISR(
            can_tx_msg_fifo.handle, &message, &HigherPriorityTaskWoken) ==
        pdTRUE)
    {
        // Transmit one CAN message in queue
        SharedCan_TransmitDataCan(message.std_id, message.dlc, message.data);
    }
    if (HigherPriorityTaskWoken != pdFALSE)
    {
        taskYIELD();
    }
}

static void SharedCan_EnqueueFifoOverflowError(void)
{
    struct CanTxMsg message;
    static uint32_t overflow_count = 0;

    // Keep track of how many times the FIFO has overflown.
    overflow_count++;

    // When we enter this function, the CAN FIFO is already full. So we forcibly
    // remove one message to make space in the CAN FIFO to transmit
    // overflow_count. I wrapped the code block with critical section to make
    // sure a context switch doesn't happen in between an element being dequeued
    // and this task enqueuing overflow_count.
    taskENTER_CRITICAL();
    xQueueReceive(can_tx_msg_fifo.handle, &message, 0);
    memcpy(&message.data, &overflow_count, sizeof(overflow_count));
    message.dlc    = CAN_TX_FIFO_OVERFLOW_DLC;
    message.std_id = CAN_TX_FIFO_OVERFLOW_STDID;
    xQueueSendToFront(can_tx_msg_fifo.handle, &message, 0);
    taskEXIT_CRITICAL();
}

static void SharedCan_BroadcastSystemReboot(void)
{
    // Since we do not know what CAN message is being used as the overflow
    // message, we just use the maximum size allowed for the message data
    SharedCan_TransmitDataCan(
        PCB_STARTUP_STDID, PCB_STARTUP_DLC, PCB_STARTUP_DATA);
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void SharedCan_TransmitDataCan(uint32_t std_id, uint32_t dlc, void *data)
{
    // Indicates the mailbox used for transmission, not currently used
    uint32_t mailbox = 0;

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
#ifndef STM32F042x6
    tx_header.TransmitGlobalTime = DISABLE;
#endif

#ifdef STM32F042x6
    // Copy over data to send
    memcpy(&(tx_header.Data[0]), &data[0], dlc * sizeof(uint8_t));

    // Copy over the header to send
    hcan.pTxMsg = &tx_header;
#endif

// If no mailbox is available or an error occured
#ifdef STM32F042x6
    if (HAL_CAN_Transmit_IT(&hcan) != HAL_OK)
#else
    if (HAL_CAN_AddTxMessage(&hcan, &tx_header, (uint8_t *)data, &mailbox) !=
        HAL_OK)
#endif
    {
        // Populate CAN TX message with CAN header and data
        struct CanTxMsg tx_msg;
        memset(&tx_msg, 0, sizeof(tx_msg));
        tx_msg.std_id = std_id;
        tx_msg.dlc    = dlc;
        memcpy(&tx_msg.data, data, tx_msg.dlc);

        if (xQueueSendToBack(can_tx_msg_fifo.handle, &tx_msg, 0) != pdTRUE)
        {
            SharedCan_EnqueueFifoOverflowError();
        }
    }
}

void SharedCan_StartCanInInterruptMode(
    CAN_HandleTypeDef *        hcan,
    CanMaskFilterConfig_Struct filters[],
    uint32_t                   num_of_filters)
{
    if (SharedCan_InitializeFilters(filters, num_of_filters) != SUCCESS)
    {
        Error_Handler();
    }

    uint32_t active_interrupts =
#ifdef STM32F042x6
        CAN_IT_TME | CAN_IT_FMP0 | CAN_IT_FMP1;
#else
        CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING |
        CAN_IT_RX_FIFO1_MSG_PENDING;
#endif

#ifdef STM32F042x6
    __HAL_CAN_ENABLE_IT(hcan, active_interrupts);
    // Reserve space for the Rx message. This is a bit hacky, but given
    // that we're soon dropping support for F0 boards and the alternative
    // is much more complicated, it does the job.
    static CanRxMsgTypeDef pRxMsg;
    hcan->pRxMsg = &pRxMsg;
#else
    if (HAL_CAN_ActivateNotification(hcan, active_interrupts) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_CAN_Start(hcan) != HAL_OK)
    {
        Error_Handler();
    }
#endif

    can_tx_msg_fifo.handle = xQueueCreateStatic(
        CAN_TX_MSG_FIFO_LENGTH, CAN_TX_MSG_FIFO_ITEM_SIZE,
        can_tx_msg_fifo.storage, &can_tx_msg_fifo.state);
    configASSERT(can_tx_msg_fifo.handle);

    SharedCan_BroadcastSystemReboot();
}

__weak void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    /* NOTE: This function Should not be modified, when the callback is needed,
              the Can_RxCommonCallback could be implemented in the Can.c file */
    UNUSED(hcan);
    UNUSED(rx_fifo);
}

// TODO (Issue #297): Use this function in all the boards
HAL_StatusTypeDef SharedCan_ReceiveDataCan(
    CAN_HandleTypeDef *hcan,
    uint32_t           rx_fifo,
    struct CanRxMsg *  rx_msg)
{
    HAL_StatusTypeDef status = HAL_ERROR;
#ifdef STM32F042x6

    status |= HAL_CAN_Receive_IT(hcan, rx_fifo);

    rx_msg->rx_header = *(hcan->pRxMsg);

    // Copy over the data from the received message
    memcpy(&(rx_msg->data[0]), &(hcan->pRxMsg->Data[0]), 8 * sizeof(uint8_t));

#else
    status |= HAL_CAN_GetRxMessage(
        hcan, rx_fifo, &rx_msg->rx_header, &rx_msg->data[0]);
#endif

    return status;
}

#ifdef STM32F042x6

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All receive mailbox interrupts shall be handled in the same way */
    /* NOTE: We just set the hardware FIFO to 0 here because the F0 board does
             not give it to us. */
    Can_RxCommonCallback(hcan, hcan->pRxMsg->FIFONumber);
}

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All transmit mailbox interrupts shall be handled in the same way */
    Can_TxCommonCallback(hcan);
}

#else

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

#endif
