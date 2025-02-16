#include "hw_can.h"
#undef NDEBUG // TODO remove this in favour of always_assert
#include <assert.h>
#include "io_log.h"
#include "io_time.h"
#include "io_canQueue.h"

void hw_can_init(const CanHandle *can_handle)
{
    // Configure a single filter bank that accepts any message.
    FDCAN_FilterTypeDef filter;
    filter.IdType       = FDCAN_STANDARD_ID; // 11 bit ID
    filter.FilterIndex  = 0;
    filter.FilterType   = FDCAN_FILTER_MASK;
    filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    filter.FilterID1    = 0;          // Standard CAN ID bits [10:0]
    filter.FilterID2    = 0x1FFFFFFF; // Mask bits for Standard CAN ID

    // Configure and initialize hardware filter.
    assert(HAL_FDCAN_ConfigFilter(can_handle->hcan, &filter) == HAL_OK);

    // Configure interrupt mode for CAN peripheral.
    assert(
        HAL_FDCAN_ActivateNotification(
            can_handle->hcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) == HAL_OK);
    assert(HAL_FDCAN_ActivateNotification(can_handle->hcan, FDCAN_IT_BUS_OFF, 0) == HAL_OK);

    // Start the FDCAN peripheral.
    assert(HAL_FDCAN_Start(can_handle->hcan) == HAL_OK);
}

void hw_can_deinit(const CanHandle *can_handle)
{
    assert(HAL_FDCAN_Stop(can_handle->hcan) == HAL_OK);
    assert(HAL_FDCAN_DeInit(can_handle->hcan) == HAL_OK);
}

bool hw_can_transmit(const CanHandle *can_handle, CanMsg *msg)
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

    while (HAL_FDCAN_GetTxFifoFreeLevel(can_handle->hcan) == 0U)
        ;

    return HAL_FDCAN_AddMessageToTxFifoQ(can_handle->hcan, &tx_header, msg->data) == HAL_OK;
}

bool hw_can_receive(const CanHandle *can_handle, const uint32_t rx_fifo, CanMsg *msg)
{
    FDCAN_RxHeaderTypeDef header;
    if (HAL_FDCAN_GetRxMessage(can_handle->hcan, rx_fifo, &header, msg->data) != HAL_OK)
    {
        return false;
    }

    msg->std_id    = header.Identifier;
    msg->dlc       = header.DataLength >> 16; // Data length code needs to be un-shifted by 16 bits.
    msg->timestamp = io_time_getCurrentMs();

    return true;
}

// ReSharper disable once CppParameterMayBeConst
void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
    // assert(hfdcan == can_handle->hcan);
    LOG_INFO("FDCAN detected an error");
    if ((ErrorStatusITs & FDCAN_IT_BUS_OFF) != RESET)
    {
        FDCAN_ProtocolStatusTypeDef protocolStatus;
        HAL_FDCAN_GetProtocolStatus(hfdcan, &protocolStatus);
        if (protocolStatus.BusOff)
        {
            CLEAR_BIT(hfdcan->Instance->CCCR, FDCAN_CCCR_INIT);
        }
    }
}

uint8_t handle_to_bus(FDCAN_HandleTypeDef *hfdcan);

static void handle(FDCAN_HandleTypeDef *hfdcan)
{
    CanMsg rx_msg;
    if (!hw_can_receive(hfdcan, FDCAN_RX_FIFO0, &rx_msg))
        // Early return if RX msg is unavailable.
        return;
    io_canQueue_pushRx(&rx_msg);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, const uint32_t RxFifo0ITs)
{
    UNUSED(RxFifo0ITs);
    handle(hfdcan);
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, const uint32_t RxFifo1ITs)
{
    UNUSED(RxFifo1ITs);
    handle(hfdcan);
}
