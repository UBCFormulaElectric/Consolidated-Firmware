#include "hw_can.h"
#include "io_can.h"
#include <assert.h>

static CanHandle *handle;

void hw_can_init(CanHandle *can_handle)
{
    handle = can_handle;
    FDCAN_FilterTypeDef filter;

    filter.IdType       = FDCAN_STANDARD_ID; // 11 bits
    filter.FilterIndex  = 0;
    filter.FilterType   = FDCAN_FILTER_MASK;
    filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    filter.FilterID1    = 0;          // Standard CAN ID bits [10:0]
    filter.FilterID2    = 0x1FFFFFFF; // Mask bits for Standard CAN ID

    // Configure and initialize hardware filter.
    assert(HAL_FDCAN_ConfigFilter(handle, &filter) == HAL_OK);

    HAL_FDCAN_ActivateNotification(
        handle, FDCAN_IT_TX_COMPLETE, FDCAN_TX_BUFFER0 | FDCAN_TX_BUFFER1 | FDCAN_TX_BUFFER2);

    // Start the FDCAN peripheral.
    assert(HAL_FDCAN_Start(handle) == HAL_OK);
}

void hw_can_deinit()
{
    assert(HAL_FDCAN_Stop(handle) == HAL_OK);
    assert(HAL_FDCAN_DeInit(handle) == HAL_OK);
}

bool hw_can_transmit(const CanMsg *msg)
{
    FDCAN_TxHeaderTypeDef tx_header;

    tx_header.Identifier          = msg->std_id;
    tx_header.DataLength          = msg->dlc;
    tx_header.TxFrameType         = FDCAN_DATA_FRAME;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch       = FDCAN_BRS_OFF;
    tx_header.FDFormat            = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;

    while (HAL_FDCAN_GetTxFifoFreeLevel(handle) == 0U)
        ;

    uint32_t fifo = FDCAN_RX_FIFO0;
    return HAL_FDCAN_AddMessageToTxFifoQ(handle, &tx_header, (uint8_t *)msg->data) == HAL_OK;
}

bool hw_can_receive(uint32_t rx_fifo, CanMsg *msg)
{
    FDCAN_RxHeaderTypeDef header;
    if (HAL_FDCAN_GetRxMessage(handle, rx_fifo, &header, msg->data) != HAL_OK)
    {
        return false;
    }

    msg->std_id = header.Identifier;
    msg->dlc    = header.DataLength;
    return true;
}

void HAL_CAN_RxFifo0MsgPendingCallback(FDCAN_HandleTypeDef *hcan)
{
    io_can_msgReceivedCallback(FDCAN_RX_FIFO0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(FDCAN_HandleTypeDef *hcan)
{
    io_can_msgReceivedCallback(FDCAN_RX_FIFO1);
}
