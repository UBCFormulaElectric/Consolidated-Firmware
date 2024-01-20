#include "hw_can.h"
#include "io_can.h"
#include <assert.h>

static CanHandle *handle;

void hw_can_init(CanHandle *can_handle, MsgReceivedCallback callback)
{
    handle = can_handle;

    // Configure a single filter bank that accepts any message.
    FDCAN_FilterTypeDef filter;
    filter.IdType       = FDCAN_STANDARD_ID; // 11 bit ID
    filter.FilterIndex  = 0;
    filter.FilterType   = FDCAN_FILTER_MASK;
    filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    filter.FilterID1    = 0;          // Standard CAN ID bits [10:0]
    filter.FilterID2    = 0x1FFFFFFF; // Mask bits for Standard CAN ID

    // Configure and initialize hardware filter.
    assert(HAL_FDCAN_ConfigFilter(handle->can, &filter) == HAL_OK);

    // Configure interrupt mode for CAN peripheral.
    assert(
        HAL_FDCAN_ActivateNotification(handle->can, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) ==
        HAL_OK);

    // Start the FDCAN peripheral.
    assert(HAL_FDCAN_Start(handle->can) == HAL_OK);

    if (!callback)
        handle->callback = callback;
    else
        handle->callback = io_can_msgReceivedCallback;
}

void hw_can_deinit()
{
    assert(HAL_FDCAN_Stop(handle->can) == HAL_OK);
    assert(HAL_FDCAN_DeInit(handle->can) == HAL_OK);
}

bool hw_can_transmit(const CanMsg *msg)
{
    FDCAN_TxHeaderTypeDef tx_header;
    tx_header.Identifier          = msg->std_id;
    tx_header.IdType              = FDCAN_STANDARD_ID;
    tx_header.TxFrameType         = FDCAN_DATA_FRAME;
    tx_header.DataLength          = msg->dlc << 16; // Data length code needs to be shifted by 16 bits.
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch       = FDCAN_BRS_OFF;
    tx_header.FDFormat            = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker       = 0;

    while (HAL_FDCAN_GetTxFifoFreeLevel(handle->can) == 0U)
        ;

    return HAL_FDCAN_AddMessageToTxFifoQ(handle->can, &tx_header, (uint8_t *)msg->data) == HAL_OK;
}

bool hw_can_receive(uint32_t rx_fifo, CanMsg *msg)
{
    FDCAN_RxHeaderTypeDef header;
    if (HAL_FDCAN_GetRxMessage(handle->can, rx_fifo, &header, msg->data) != HAL_OK)
    {
        return false;
    }

    msg->std_id = header.Identifier;
    msg->dlc    = header.DataLength >> 16; // Data length code needs to be un-shifted by 16 bits.

    return true;
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    io_can_msgReceivedCallback(FDCAN_RX_FIFO0);
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
    io_can_msgReceivedCallback(FDCAN_RX_FIFO1);
}
