#include "hw_can.h"
#include <assert.h>
#include "io_can.h"

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
#define INIT_MASKMODE_16BIT_FiRx(std_id, rtr, ide, ext_id)                                                             \
    ((((uint32_t)(std_id) << 5U) & 0xFFE0) | (((uint32_t)(rtr) << 4U) & 0x0010) | (((uint32_t)(ide) << 3U) & 0x0008) | \
     (((uint32_t)(ext_id) << 0U) & 0x0007))

// Open CAN filter that accepts any CAN message as long as it uses Standard CAN
// ID and is a data frame.
#define CAN_ExtID_NULL 0 // Set CAN Extended ID to 0 because we are not using it.
#define MASKMODE_16BIT_ID_OPEN INIT_MASKMODE_16BIT_FiRx(0x0, CAN_ID_STD, CAN_RTR_DATA, CAN_ExtID_NULL)
#define MASKMODE_16BIT_MASK_OPEN INIT_MASKMODE_16BIT_FiRx(0x0, 0x1, 0x1, 0x0)

static const CanHandle *handle;

void hw_can_init(const CanHandle *can_handle)
{
    handle = can_handle;

    // Configure a single filter bank that accepts any message.
    CAN_FilterTypeDef filter;
    filter.FilterMode           = CAN_FILTERMODE_IDMASK;
    filter.FilterScale          = CAN_FILTERSCALE_16BIT;
    filter.FilterActivation     = CAN_FILTER_ENABLE;
    filter.FilterIdLow          = MASKMODE_16BIT_ID_OPEN;
    filter.FilterMaskIdLow      = MASKMODE_16BIT_MASK_OPEN;
    filter.FilterIdHigh         = MASKMODE_16BIT_ID_OPEN;
    filter.FilterMaskIdHigh     = MASKMODE_16BIT_MASK_OPEN;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterBank           = 0;

    // Configure and initialize hardware filter.
    assert(HAL_CAN_ConfigFilter(handle->can, &filter) == HAL_OK);

    // Configure interrupt mode for CAN peripheral.
    assert(
        HAL_CAN_ActivateNotification(
            handle->can, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING) ==
        HAL_OK);

    // Start the CAN peripheral.
    assert(HAL_CAN_Start(handle->can) == HAL_OK);
}

void hw_can_deinit()
{
    assert(HAL_CAN_Stop(handle->can) == HAL_OK);
    assert(HAL_CAN_DeInit(handle->can) == HAL_OK);
}

bool hw_can_transmit(const CanMsg *msg)
{
    CAN_TxHeaderTypeDef tx_header;

    tx_header.DLC   = msg->dlc;
    tx_header.StdId = msg->std_id;

    // The standard 11-bit CAN identifier is more than sufficient, so we disable
    // Extended CAN IDs by setting this field to zero.
    tx_header.ExtId = CAN_ExtID_NULL;

    // This field can be either Standard CAN or Extended CAN. See .ExtID to see
    // why we don't want Extended CAN.
    tx_header.IDE = CAN_ID_STD;

    // This field can be either Data Frame or Remote Frame. For our
    // purpose, we only ever transmit Data Frames.
    tx_header.RTR = CAN_RTR_DATA;

    // Enabling this gives us a tick-based timestamp which we do not need. Plus,
    // it would take up 2 bytes of the CAN payload. So we disable the timestamp.
    tx_header.TransmitGlobalTime = DISABLE;

    // Spin until a TX mailbox becomes available.
    while (HAL_CAN_GetTxMailboxesFreeLevel(handle->can) == 0U)
        ;

    // Indicates the mailbox used for transmission, not currently used.
    uint32_t mailbox = 0;
    return HAL_CAN_AddTxMessage(handle->can, &tx_header, msg->data, &mailbox) == HAL_OK;
}

bool hw_can_receive(uint32_t rx_fifo, CanMsg *msg)
{
    CAN_RxHeaderTypeDef header;
    if (HAL_CAN_GetRxMessage(handle->can, rx_fifo, &header, msg->data) != HAL_OK)
    {
        return false;
    }

    // Copy metadata from HAL's CAN message struct into our custom CAN
    // message struct
    msg->std_id = header.StdId;
    msg->dlc    = header.DLC;
    return true;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CanMsg rx_msg;
    if (!hw_can_receive(CAN_RX_FIFO0, &rx_msg))
    {
        // Early return if RX msg is unavailable.
        return;
    }

    handle->can_msg_received_callback(&rx_msg);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CanMsg rx_msg;
    if (!hw_can_receive(CAN_RX_FIFO1, &rx_msg))
    {
        // Early return if RX msg is unavailable.
        return;
    }

    handle->can_msg_received_callback(&rx_msg);
}
