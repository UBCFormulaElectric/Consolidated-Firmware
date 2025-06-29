#include "app_utils.h"
#include "hw_fdcan.h"
#include <stm32h7xx_hal_fdcan.h>
#undef NDEBUG // TODO remove this in favour of always_assert
#include <assert.h>
#include <cmsis_os2.h>
#include <FreeRTOS.h>
#include <task.h>
#include <string.h>

#include "io_log.h"
#include "io_time.h"

void hw_can_init(CanHandle *can_handle)
{
    assert(!can_handle->ready);
    // Configure a single filter bank that accepts any message.
    FDCAN_FilterTypeDef filter;
    filter.IdType           = FDCAN_EXTENDED_ID; // 29 bit ID
    filter.FilterIndex      = 0;
    filter.FilterType       = FDCAN_FILTER_MASK;
    filter.FilterConfig     = FDCAN_FILTER_TO_RXFIFO0;
    filter.FilterID1        = 0x00000000; // Standard CAN ID bits [28:0]
    filter.FilterID2        = 0x1FFFFFFF; // Mask bits for Extended CAN ID
    filter.IsCalibrationMsg = 0;
    filter.RxBufferIndex    = 0;

    // Configure and initialize hardware filter.
    assert(HAL_FDCAN_ConfigFilter(can_handle->hcan, &filter) == HAL_OK);

    // Configure interrupt mode for CAN peripheral.
    assert(
        HAL_FDCAN_ActivateNotification(
            can_handle->hcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_RX_FIFO1_NEW_MESSAGE | FDCAN_IT_BUS_OFF,
            FDCAN_TX_BUFFER0) == HAL_OK);

    // Start the FDCAN peripheral.
    assert(HAL_FDCAN_Start(can_handle->hcan) == HAL_OK);
    can_handle->ready = true;
}

void hw_can_deinit(const CanHandle *can_handle)
{
    assert(HAL_FDCAN_Stop(can_handle->hcan) == HAL_OK);
    assert(HAL_FDCAN_DeInit(can_handle->hcan) == HAL_OK);
}

static ExitCode tx(CanHandle *can_handle, FDCAN_TxHeaderTypeDef tx_header, CanMsg *msg)
{
    while (HAL_FDCAN_GetTxFifoFreeLevel(can_handle->hcan) == 0U)
        ;
    return hw_utils_convertHalStatus(HAL_FDCAN_AddMessageToTxFifoQ(can_handle->hcan, &tx_header, msg->data.data8));
}

ExitCode hw_can_transmit(CanHandle *can_handle, CanMsg *msg)
{
    assert(can_handle->ready);
    FDCAN_TxHeaderTypeDef tx_header;
    tx_header.Identifier          = msg->std_id;
    tx_header.IdType              = (msg->std_id > MAX_11_BITS_VALUE) ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
    tx_header.TxFrameType         = FDCAN_DATA_FRAME;
    tx_header.DataLength          = msg->dlc << 16; // Data length code needs to be shifted by 16 bits.
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch       = FDCAN_BRS_OFF;
    tx_header.FDFormat            = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker       = 0;
    return tx(can_handle, tx_header, msg);
}

ExitCode hw_fdcan_transmit(CanHandle *can_handle, CanMsg *msg)
{
    assert(can_handle->ready);

    uint32_t dlc;
    if (msg->dlc <= 8)
    {
        dlc = msg->dlc << 16; // Data length code needs to be shifted by 16 bits.
    }
    else if (msg->dlc <= 12)
    {
        dlc = FDCAN_DLC_BYTES_12;
    }
    else if (msg->dlc <= 16)
    {
        dlc = FDCAN_DLC_BYTES_16;
    }
    else if (msg->dlc <= 20)
    {
        dlc = FDCAN_DLC_BYTES_20;
    }
    else if (msg->dlc <= 24)
    {
        dlc = FDCAN_DLC_BYTES_24;
    }
    else if (msg->dlc <= 32)
    {
        dlc = FDCAN_DLC_BYTES_32;
    }
    else if (msg->dlc <= 48)
    {
        dlc = FDCAN_DLC_BYTES_48;
    }
    else
    {
        dlc = FDCAN_DLC_BYTES_64;
    }

    FDCAN_TxHeaderTypeDef tx_header;
    tx_header.Identifier          = msg->std_id;
    tx_header.IdType              = (msg->std_id > MAX_11_BITS_VALUE) ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
    tx_header.TxFrameType         = FDCAN_DATA_FRAME;
    tx_header.DataLength          = dlc;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = (can_handle->hcan->Init.FrameFormat == FDCAN_FRAME_FD_BRS) ? FDCAN_BRS_ON : FDCAN_BRS_OFF;
    tx_header.FDFormat      = FDCAN_FD_CAN;
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker      = 0;
    return tx(can_handle, tx_header, msg);
}

ExitCode hw_fdcan_receive(const CanHandle *can_handle, const uint32_t rx_fifo, CanMsg *msg)
{
    assert(can_handle->ready);
    FDCAN_RxHeaderTypeDef header;

    RETURN_IF_ERR_SILENT(
        hw_utils_convertHalStatus(HAL_FDCAN_GetRxMessage(can_handle->hcan, rx_fifo, &header, msg->data.data8)));
    msg->std_id    = header.Identifier;
    msg->dlc       = header.DataLength >> 16; // Data length code needs to be un-shifted by 16 bits.
    msg->timestamp = io_time_getCurrentMs();
    msg->bus       = can_handle->bus_num;

    return EXIT_CODE_OK;
}

// ReSharper disable once CppParameterMayBeConst
void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
    LOG_INFO("FDCAN on bus %d detected an error: %x", hw_can_getHandle(hfdcan)->bus_num, ErrorStatusITs);
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

static ExitCode handleCallback(FDCAN_HandleTypeDef *hfdcan, uint8_t fifo)
{
    const CanHandle *handle = hw_can_getHandle(hfdcan);

    CanMsg rx_msg;
    RETURN_IF_ERR_SILENT(hw_fdcan_receive(handle, fifo, &rx_msg));

    if (handle->receive_callback == NULL)
    {
        LOG_ERROR("CAN has no callback configured!");
        return EXIT_CODE_INVALID_ARGS;
    }

    handle->receive_callback(&rx_msg);
    return EXIT_CODE_OK;
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, const uint32_t RxFifo0ITs)
{
    UNUSED(RxFifo0ITs);
    while (IS_EXIT_OK(handleCallback(hfdcan, FDCAN_RX_FIFO0)))
        ;
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, const uint32_t RxFifo1ITs)
{
    UNUSED(RxFifo1ITs);
    while (IS_EXIT_OK(handleCallback(hfdcan, FDCAN_RX_FIFO1)))
        ;
}
