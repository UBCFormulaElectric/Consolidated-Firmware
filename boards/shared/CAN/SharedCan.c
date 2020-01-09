/******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include "auto_generated/App_CanTx.h"
#include "auto_generated/App_CanRx.h"
#include "SharedCan.h"
#include "BoardSpecifics.h"
#include "SharedAssert.h"
#include "SharedFreeRTOS.h"
#include "SharedAssert.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
/** @brief Size of a message in the CAN TX queue */
#define CAN_TX_MSG_FIFO_ITEM_SIZE sizeof(struct CanMsg)

/**
 * @brief Number of messages in the CAN TX queue. This value should be increased
 *        if CAN TX overflow happens frequently.
 */
#define CAN_TX_MSG_FIFO_LENGTH 20

/** @brief Size of a message in the CAN RX queue */
#define CAN_RX_MSG_FIFO_ITEM_SIZE sizeof(struct CanMsg)

/**
 * @brief Number of messages in the CAN RX queue. This value should be increased
 *        if CAN RX overflow happens frequently.
 */
#define CAN_RX_MSG_FIFO_LENGTH 20

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
/** @brief Board-specific struct type for the CAN startup message payload */
#define BOARD_STARTUP_STRUCT(board) _BOARD_STARTUP_STRUCT(board)
#define _BOARD_STARTUP_STRUCT(board) struct CanMsgs_##board##_startup_t

/** @brief Board-specific function to transmit the CAN startup message */
#define ENQUEUE_TRANSMIT_BOARD_STARTUP(BOARD, ARG) \
    _ENQUEUE_BOARD_STARTUP(BOARD, ARG)
#define _ENQUEUE_BOARD_STARTUP(BOARD, ARG) \
    App_CanTx_EnqueueNonPeriodicMsg_##BOARD##_STARTUP(ARG)

/** @brief Board-specific struct type for the CAN TX FIFO overflow payload */
#define CAN_TX_FIFO_OVERFLOW_STRUCT(board) _CAN_TX_FIFO_OVERFLOW_STRUCT(board)
#define _CAN_TX_FIFO_OVERFLOW_STRUCT(board) \
    CanMsgs_##board##_can_tx_fifo_overflow_t

/**
 * @brief Board-specific function to transmit the CAN TX FIFO overflow message
 */
#define FORCE_ENQUEUE_CAN_TX_FIFO_OVERFLOW(BOARD, ARG) \
    _FORCE_ENQUEUE_CAN_TX_FIFO_OVERFLOW(BOARD, ARG)
#define _FORCE_ENQUEUE_CAN_TX_FIFO_OVERFLOW(BOARD, ARG) \
    App_CanTx_ForceEnqueueNonPeriodicMsg_##BOARD##_CAN_TX_FIFO_OVERFLOW(ARG)

/** @brief Board-specific struct type for the CAN RX FIFO overflow payload */
#define CAN_RX_FIFO_OVERFLOW_STRUCT(board) _CAN_RX_FIFO_OVERFLOW_STRUCT(board)
#define _CAN_RX_FIFO_OVERFLOW_STRUCT(board) \
    CanMsgs_##board##_can_rx_fifo_overflow_t

/**
 * @brief Board-specific function to transmit the CAN RX FIFO overflow message
 */
#define FORCE_ENQUEUE_CAN_RX_FIFO_OVERFLOW(BOARD, ARG) \
    _FORCE_ENQUEUE_CAN_RX_FIFO_OVERFLOW(BOARD, ARG)
#define _FORCE_ENQUEUE_CAN_RX_FIFO_OVERFLOW(BOARD, ARG) \
    App_CanTx_ForceEnqueueNonPeriodicMsg_##BOARD##_CAN_RX_FIFO_OVERFLOW(ARG)

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
#define INIT_MASKMODE_16BIT_FiRx(std_id, rtr, ide, ext_id)                     \
    ((((uint32_t)(std_id) << 5U) & 0xFFE0) |                                   \
     (((uint32_t)(rtr) << 4U) & 0x0010) | (((uint32_t)(ide) << 3U) & 0x0008) | \
     (((uint32_t)(ext_id) << 0U) & 0x0007))

// Open CAN filter that accepts any CAN message as long as it uses Standard CAN
// ID and is a data frame.
#define MASKMODE_16BIT_ID_OPEN \
    INIT_MASKMODE_16BIT_FiRx(0x0, CAN_ID_STD, CAN_RTR_DATA, CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_OPEN INIT_MASKMODE_16BIT_FiRx(0x0, 0x1, 0x1, 0x0)

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
    .state   = { { 0 } },
    .handle  = NULL,
};

/** @brief FIFO for buffering CAN RX messages */
static uint8_t
    can_rx_msg_fifo_storage[CAN_TX_MSG_FIFO_LENGTH * CAN_TX_MSG_FIFO_ITEM_SIZE];

static struct FreeRTOSStaticQueue can_rx_msg_fifo = {
    .storage = &can_rx_msg_fifo_storage[0],
    .state   = { { 0 } },
    .handle  = NULL,
};

/** @brief Handle used to interface with underlying CAN hardware */
static CAN_HandleTypeDef *sharedcan_hcan = NULL;

/** @brief Boolean flag indicating whether this library is initialized */
static bool sharedcan_initialized = false;

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/
/**
 * @brief Transmits a CAN message
 * @param can_tx_msg CAN message to transmit
 */
static HAL_StatusTypeDef Io_TransmitCanMessage(struct CanMsg *can_tx_msg);

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

/**
 * Initializes the filters on the given CAN interface to allow all msgs through
 * @param hcan The interface to set the fully open filter on
 * @return SUCCESS if success, otherwise an error code
 */
static ErrorStatus Io_InitializeAllOpenFilters(CAN_HandleTypeDef *hcan);

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

static ErrorStatus Io_InitializeAllOpenFilters(CAN_HandleTypeDef *hcan)
{
    shared_assert(hcan != NULL);

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

static HAL_StatusTypeDef Io_TransmitCanMessage(struct CanMsg *can_tx_msg)
{
    shared_assert(sharedcan_initialized == true);
    shared_assert(can_tx_msg != NULL);

    // Indicates the mailbox used for transmission, not currently used
    uint32_t mailbox = 0;

    CAN_TxHeaderTypeDef tx_header;

    tx_header.DLC   = can_tx_msg->dlc;
    tx_header.StdId = can_tx_msg->std_id;

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

    return HAL_CAN_AddTxMessage(
        sharedcan_hcan, &tx_header, can_tx_msg->data, &mailbox);
}

static inline void Io_CanRxCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    shared_assert(sharedcan_initialized == true);
    shared_assert(hcan != NULL);

    BaseType_t          xHigherPriorityTaskWoken = pdFALSE;
    CAN_RxHeaderTypeDef rx_header;
    struct CanMsg       rx_msg;

    if (HAL_CAN_GetRxMessage(hcan, rx_fifo, &rx_header, &rx_msg.data[0]) ==
        HAL_OK)
    {
        // Copy metadata from HAL's CAN message struct into our custom CAN
        // message struct
        rx_msg.std_id = rx_header.StdId;
        rx_msg.dlc    = rx_header.DLC;

        // We defer reading the CAN RX message to a task by storing the message
        // on the CAN RX queue
        if (xQueueSendToBackFromISR(
                can_rx_msg_fifo.handle, &rx_msg, &xHigherPriorityTaskWoken) !=
            pdPASS)
        {
            // Log the number of RX FIFO overflow over CAN
            static struct CAN_RX_FIFO_OVERFLOW_STRUCT(BOARD_NAME_LOWERCASE)
                canrx_overflow_count = { .overflow_count = 0 };

            canrx_overflow_count.overflow_count++;

            FORCE_ENQUEUE_CAN_RX_FIFO_OVERFLOW(
                BOARD_NAME_UPPERCASE, &canrx_overflow_count);
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void SharedCan_Init(CAN_HandleTypeDef *hcan)
{
    shared_assert(hcan != NULL);

    // Initialize CAN TX software queue
    can_tx_msg_fifo.handle = xQueueCreateStatic(
        CAN_TX_MSG_FIFO_LENGTH, CAN_TX_MSG_FIFO_ITEM_SIZE,
        can_tx_msg_fifo.storage, &can_tx_msg_fifo.state);
    shared_assert(can_tx_msg_fifo.handle != NULL);

    // Initialize CAN RX software queue
    can_rx_msg_fifo.handle = xQueueCreateStatic(
        CAN_RX_MSG_FIFO_LENGTH, CAN_RX_MSG_FIFO_ITEM_SIZE,
        can_rx_msg_fifo.storage, &can_rx_msg_fifo.state);
    shared_assert(can_rx_msg_fifo.handle != NULL);

    // Initialize CAN RX hardware filters
    shared_assert(Io_InitializeAllOpenFilters(hcan) == SUCCESS);

    // Configure interrupt mode for CAN peripheral
    shared_assert(
        HAL_CAN_ActivateNotification(
            hcan, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING |
                      CAN_IT_RX_FIFO1_MSG_PENDING) == HAL_OK);

    // Enable the CAN peripheral
    shared_assert(HAL_CAN_Start(hcan) == HAL_OK);

    // Save a copy of the CAN handle that shall be used in this library
    sharedcan_hcan = hcan;

    // Set a boolean to indicate that the library is ready be used
    sharedcan_initialized = true;

    // Broadcast a CAN message to indicate that the system has rebooted
    BOARD_STARTUP_STRUCT(BOARD_NAME_LOWERCASE) pcb_startup = { .dummy = 0 };
    ENQUEUE_TRANSMIT_BOARD_STARTUP(BOARD_NAME_UPPERCASE, &pcb_startup);
}

void App_SharedCan_TxMessageQueueSendtoBack(struct CanMsg *message)
{
    shared_assert(sharedcan_initialized == true);
    shared_assert(message != NULL);

    if (xQueueSendToBack(can_tx_msg_fifo.handle, message, 0) != pdTRUE)
    {
        // Log the number of TX FIFO overflow over CAN
        static struct CAN_TX_FIFO_OVERFLOW_STRUCT(BOARD_NAME_LOWERCASE)
            cantx_overflow_count = { .overflow_count = 0 };

        cantx_overflow_count.overflow_count++;

        FORCE_ENQUEUE_CAN_TX_FIFO_OVERFLOW(
            BOARD_NAME_UPPERCASE, &cantx_overflow_count);
    }
}

void App_SharedCan_TxMessageQueueForceSendToFront(struct CanMsg *message)
{
    shared_assert(sharedcan_initialized == true);
    shared_assert(message != NULL);

    struct CanMsg dummy_buffer;
    taskENTER_CRITICAL();
    if (uxQueueSpacesAvailable(can_tx_msg_fifo.handle) == 0)
    {
        if (xPortIsInsideInterrupt())
        {
            xQueueReceiveFromISR(can_tx_msg_fifo.handle, &dummy_buffer, NULL);
        }
        else
        {
            xQueueReceive(can_tx_msg_fifo.handle, &dummy_buffer, 0);
        }
    }

    if (xPortIsInsideInterrupt())
    {
        xQueueSendToFrontFromISR(can_tx_msg_fifo.handle, message, NULL);
    }
    else
    {
        xQueueSendToFront(can_tx_msg_fifo.handle, message, 0);
    }

    taskEXIT_CRITICAL();
}

void App_SharedCan_ReadRxMessagesIntoTableFromTask(void)
{
    shared_assert(sharedcan_initialized == true);

    struct CanMsg rx_msg;

    // Get a message from the RX queue and process it, else block forever.
    if (xQueueReceive(can_rx_msg_fifo.handle, &rx_msg, portMAX_DELAY) == pdTRUE)
    {
        App_CanRx_ReadMessageIntoTableFromTask(rx_msg.std_id, &rx_msg.data[0]);
    }
}

void App_SharedCan_TransmitEnqueuedCanTxMessagesFromTask(void)
{
    shared_assert(sharedcan_initialized == true);

    struct CanMsg tx_msg;

    while (HAL_CAN_GetTxMailboxesFreeLevel(sharedcan_hcan) == 0)
    {
        // Busy-wait until a TX mailbox is available. This delay should be
        // negligible.
    }

    // Get a message from the CAN TX queue and transmit it, else block forever.
    if (xQueuePeek(can_tx_msg_fifo.handle, &tx_msg, portMAX_DELAY) == pdTRUE)
    {
        if (Io_TransmitCanMessage(&tx_msg) == HAL_OK)
        {
            // Remove the message from CAN TX queue if we were able to transmit
            // it. This should never fail. If it ever does, then the CAN TX
            // queue is probably being consumed somewhere else by mistake.
            shared_assert(
                xQueueReceive(can_tx_msg_fifo.handle, &tx_msg, 0) == pdTRUE);
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
