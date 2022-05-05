#include <assert.h>

#include "Io_CanRx.h"
#include "Io_CanTx.h"
#include "Io_SharedCan.h"
#include "Io_SharedFreeRTOS.h"

#define CAN_TX_MSG_FIFO_ITEM_SIZE sizeof(struct CanMsg)
#define CAN_TX_MSG_FIFO_LENGTH 20

#define CAN_RX_MSG_FIFO_ITEM_SIZE sizeof(struct CanMsg)
#define CAN_RX_MSG_FIFO_LENGTH 20

// The following filter IDs/masks must be used with 16-bit Filter Scale
// (FSCx = 0) and Identifier Mask Mode (FBMx = 0). In this mode, the identifier
// registers are associated with mask registers specifying which bits of the
// identifier are handled as "don't care" or as "must match". For each bit in
// the mask registers, 0 = Don't Care and 1 = Must Match.
//
// Bit mapping of a 16-bit identifier register and mask register:
// Standard CAN ID [15:5] RTR[4] IDE[3] Extended CAN ID [2:0]
//
// For example, with the following filter IDs/mask:
// =======================================================
// Identifier Register:    [000 0000 0000] [0] [0] [000]
// Mask Register:          [111 1110 0000] [1] [1] [000]
// =======================================================
// The filter will accept incoming messages that match the following criteria:
// [000 000x xxxx]    [0]    [0]         [xxx]
// Standard CAN ID    RTR    IDE     Extended CAN ID

/** @brief Helper macro to initialize FiRx register in 16-bit mode */
#define INIT_MASKMODE_16BIT_FiRx(std_id, rtr, ide, ext_id)                                                             \
    ((((uint32_t)(std_id) << 5U) & 0xFFE0) | (((uint32_t)(rtr) << 4U) & 0x0010) | (((uint32_t)(ide) << 3U) & 0x0008) | \
     (((uint32_t)(ext_id) << 0U) & 0x0007))

// Open CAN filter that accepts any CAN message as long as it uses Standard CAN
// ID and is a data frame.
#define MASKMODE_16BIT_ID_OPEN INIT_MASKMODE_16BIT_FiRx(0x0, CAN_ID_STD, CAN_RTR_DATA, CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_OPEN INIT_MASKMODE_16BIT_FiRx(0x0, 0x1, 0x1, 0x0)

static uint8_t can_tx_msg_fifo_storage[CAN_TX_MSG_FIFO_LENGTH * CAN_TX_MSG_FIFO_ITEM_SIZE];

static struct StaticQueue can_tx_msg_fifo = {
    .storage = &can_tx_msg_fifo_storage[0],
    .state   = { { 0 } },
    .handle  = NULL,
};

static uint8_t can_rx_msg_fifo_storage[CAN_TX_MSG_FIFO_LENGTH * CAN_TX_MSG_FIFO_ITEM_SIZE];

static struct StaticQueue can_rx_msg_fifo = {
    .storage = &can_rx_msg_fifo_storage[0],
    .state   = { { 0 } },
    .handle  = NULL,
};

static CAN_HandleTypeDef *sharedcan_hcan = NULL;

/**
 * @brief Callback to call with the current overflow count when the TX
 *        queue overflows
 */
static void (*_tx_overflow_callback)(size_t) = NULL;

/**
 * @brief Callback to call with the current overflow count when the RX
 *        queue overflows
 */
static void (*_rx_overflow_callback)(size_t) = NULL;

static struct StaticSemaphore CanTxBinarySemaphore = {
    .handle  = NULL,
    .storage = { { 0 } },
};

/**
 * @brief Transmits a CAN message
 * @param message CAN message to transmit
 */
static HAL_StatusTypeDef Io_TransmitCanMessage(struct CanMsg *message);

/**
 * @brief  Shared callback function to be used in each RX FIFO callback
 *         (STM32F302x8's bxCAN peripheral has two - FIFO0 and FIFO1). We push
 *         the RX messages onto a queue which is intended to be consumed in user
 *         tasks.
 * @param  hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @param  rx_fifo The Rx FIFO that triggered the callback
 */
static void Io_CanRxCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo);

/*
 * @brief Consolidate CAN TX complete callback into one function
 */
static inline void Io_CanTxCompleteCallback(void);

/**
 * Initializes the filters on the given CAN interface to allow all msgs through
 * @param hcan The interface to set the fully open filter on
 * @return SUCCESS if success, otherwise an error code
 */
static ErrorStatus Io_InitializeAllOpenFilters(CAN_HandleTypeDef *hcan);

static ErrorStatus Io_InitializeAllOpenFilters(CAN_HandleTypeDef *hcan)
{
    /* Configure a single filter bank that accepts any message */
    CAN_FilterTypeDef can_filter;
    can_filter.FilterMode           = CAN_FILTERMODE_IDMASK;
    can_filter.FilterScale          = CAN_FILTERSCALE_16BIT;
    can_filter.FilterActivation     = CAN_FILTER_ENABLE;
    can_filter.FilterIdLow          = MASKMODE_16BIT_ID_OPEN;
    can_filter.FilterMaskIdLow      = MASKMODE_16BIT_MASK_OPEN;
    can_filter.FilterIdHigh         = MASKMODE_16BIT_ID_OPEN;
    can_filter.FilterMaskIdHigh     = MASKMODE_16BIT_MASK_OPEN;
    can_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    can_filter.FilterBank           = 0;

    // Configure and initialize filter bank
    if (HAL_CAN_ConfigFilter(hcan, &can_filter) != HAL_OK)
        return ERROR;
    else
        return SUCCESS;
}

static HAL_StatusTypeDef Io_TransmitCanMessage(struct CanMsg *message)
{
    // Indicates the mailbox used for transmission, not currently used
    uint32_t mailbox = 0;

    CAN_TxHeaderTypeDef tx_header;

    tx_header.DLC   = message->dlc;
    tx_header.StdId = message->std_id;

    // The standard 11-bit CAN identifier is more than sufficient, so we disable
    // Extended CAN IDs by setting this field to zero.
    tx_header.ExtId = CAN_ExtID_NULL;

    // This field can be either Standard CAN or Extended CAN. See .ExtID to see
    // why we don't want Extended CAN
    tx_header.IDE = CAN_ID_STD;

    // This field can be either Data Frame or Remote Frame. For our
    // purpose, we only ever transmit Data Frames.
    tx_header.RTR = CAN_RTR_DATA;

    // Enabling this gives us a tick-based timestamp which we do not need. Plus,
    // it would take up 2 bytes of the CAN payload. So we disable the timestamp.
    tx_header.TransmitGlobalTime = DISABLE;

    return HAL_CAN_AddTxMessage(sharedcan_hcan, &tx_header, message->data, &mailbox);
}

static inline void Io_CanRxCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    static uint32_t canrx_overflow_count = { 0 };

    CAN_RxHeaderTypeDef header;
    struct CanMsg       message;

    if (HAL_CAN_GetRxMessage(hcan, rx_fifo, &header, &message.data[0]) == HAL_OK)
    {
        // Do we care about reading this incoming message at all?
        if (Io_CanRx_FilterMessageId(header.StdId) == true)
        {
            // Copy metadata from HAL's CAN message struct into our custom CAN
            // message struct
            message.std_id = header.StdId;
            message.dlc    = header.DLC;

            // We defer reading the CAN RX message to a task by storing the
            // message on the CAN RX queue
            if (xQueueSendToBackFromISR(can_rx_msg_fifo.handle, &message, NULL) != pdPASS)
            {
                // If the RX FIFO is full, we discard the message and log the
                // overflow over CAN.
                canrx_overflow_count++;
                _rx_overflow_callback(canrx_overflow_count);
            }
        }
    }
}

static inline void Io_CanTxCompleteCallback(void)
{
    // We don't want to wake up CAN TX task if there is no message waiting to
    // be sent.
    if (uxQueueMessagesWaitingFromISR(CanTxBinarySemaphore.handle) == 0U &&
        uxQueueMessagesWaitingFromISR(can_tx_msg_fifo.handle) > 0)
    {
        xSemaphoreGiveFromISR(CanTxBinarySemaphore.handle, NULL);
    }
}

void Io_SharedCan_Init(
    CAN_HandleTypeDef *hcan,
    void (*tx_overflow_callback)(size_t),
    void (*rx_overflow_callback)(size_t))
{
    assert(hcan != NULL);
    assert(tx_overflow_callback != NULL);
    assert(rx_overflow_callback != NULL);

    _rx_overflow_callback = rx_overflow_callback;
    _tx_overflow_callback = tx_overflow_callback;

    // Initialize CAN TX software queue
    can_tx_msg_fifo.handle = xQueueCreateStatic(
        CAN_TX_MSG_FIFO_LENGTH, CAN_TX_MSG_FIFO_ITEM_SIZE, can_tx_msg_fifo.storage, &can_tx_msg_fifo.state);
    assert(can_tx_msg_fifo.handle != NULL);

    // Initialize binary semaphore for CAN TX task
    CanTxBinarySemaphore.handle = xSemaphoreCreateBinaryStatic(&CanTxBinarySemaphore.storage);
    assert(CanTxBinarySemaphore.handle);

    // Initialize CAN RX software queue
    can_rx_msg_fifo.handle = xQueueCreateStatic(
        CAN_RX_MSG_FIFO_LENGTH, CAN_RX_MSG_FIFO_ITEM_SIZE, can_rx_msg_fifo.storage, &can_rx_msg_fifo.state);
    assert(can_rx_msg_fifo.handle != NULL);

    // Initialize CAN RX hardware filters
    assert(Io_InitializeAllOpenFilters(hcan) == SUCCESS);

    // Configure interrupt mode for CAN peripheral
    assert(
        HAL_CAN_ActivateNotification(
            hcan, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING) == HAL_OK);

    // Enable the CAN peripheral
    assert(HAL_CAN_Start(hcan) == HAL_OK);

    // Save a copy of the CAN handle that shall be used in this library
    sharedcan_hcan = hcan;
}

void Io_SharedCan_TxMessageQueueSendtoBack(const struct CanMsg *message)
{
    // Track how many times the CAN TX FIFO has overflowed
    static uint32_t cantx_overflow_count = { 0 };

    if (xPortIsInsideInterrupt())
    {
        if (xQueueSendToBackFromISR(can_tx_msg_fifo.handle, message, NULL) != pdTRUE)
        {
            // If the TX FIFO is full, we discard the message and log the
            // overflow over CAN.
            cantx_overflow_count++;
            _tx_overflow_callback(cantx_overflow_count);
        }
        else if (uxQueueMessagesWaitingFromISR(CanTxBinarySemaphore.handle) == 0U)
        {
            // Give the binary semaphore only if it's not already given, or else
            // xSemaphoreGive() would fail and clutter up Tracealyzer.
            xSemaphoreGiveFromISR(CanTxBinarySemaphore.handle, NULL);
        }
    }
    else
    {
        if (xQueueSendToBack(can_tx_msg_fifo.handle, message, 0) != pdTRUE)
        {
            // If the TX FIFO is full, we discard the message and log the
            // overflow over CAN.
            cantx_overflow_count++;
            _tx_overflow_callback(cantx_overflow_count);
        }
        // Without this if-statement, xSemaphore could fail and this would
        // clutter up the Tracealyzer trace.
        else if (uxQueueMessagesWaiting(CanTxBinarySemaphore.handle) == 0U)
        {
            // Give the binary semaphore only if it's not already given, or else
            // xSemaphoreGive() would fail and clutter up Tracealyzer.
            xSemaphoreGive(CanTxBinarySemaphore.handle);
        }
    }
}

void Io_SharedCan_DequeueCanRxMessage(struct CanMsg *message)
{
    // Get a message from the RX queue and process it, else block forever.
    while (xQueueReceive(can_rx_msg_fifo.handle, message, portMAX_DELAY) != pdTRUE)
        ;
}

void Io_SharedCan_TransmitEnqueuedCanTxMessagesFromTask(void)
{
    xSemaphoreTake(CanTxBinarySemaphore.handle, portMAX_DELAY);

    while (HAL_CAN_GetTxMailboxesFreeLevel(sharedcan_hcan) > 0 && uxQueueMessagesWaiting(can_tx_msg_fifo.handle) > 0)
    {
        struct CanMsg message;

        if (xQueueReceive(can_tx_msg_fifo.handle, &message, 0) == pdTRUE)
        {
            (void)Io_TransmitCanMessage(&message);
        }
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All receive mailbox interrupts shall be handled in the same way */
    Io_CanRxCallback(hcan, CAN_RX_FIFO0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All receive mailbox interrupts shall be handled in the same way */
    Io_CanRxCallback(hcan, CAN_RX_FIFO1);
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All transmit mailbox interrupts shall be handled in the same way */
    UNUSED(hcan);
    Io_CanTxCompleteCallback();
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All transmit mailbox interrupts shall be handled in the same way */
    UNUSED(hcan);
    Io_CanTxCompleteCallback();
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    /* NOTE: All transmit mailbox interrupts shall be handled in the same way */
    UNUSED(hcan);
    Io_CanTxCompleteCallback();
}
