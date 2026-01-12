#include "hw_can.hpp"
#include <io_canMsg.hpp>
#include <util_errorCodes.hpp>
#undef NDEBUG // TODO remove this in favour of always_assert
#include <cassert>
#include <FreeRTOS.h>
#include <task.h>

#include "io_log.hpp"
#include "io_time.hpp"
#include "hw_utils.hpp"

#if defined(STM32H733xx)
#include "stm32h7xx_hal_fdcan.h"
#elif defined(STM32H562xx)
#include "stm32h5xx_hal_fdcan.h"
#endif

ExitCode hw::fdcan::tx(FDCAN_TxHeaderTypeDef &tx_header, io::CanMsg *msg) 
{
    for (uint32_t poll = 0; HAL_FDCAN_GetTxFifoFreeLevel(hfdcan) == 0U;)
    {
        // the polling is here because if the CAN mailbox is temporarily blocked, we don't want to incur the
        // overhead of context switching
        if (poll <= 1000)
        {
            poll++;
            continue;
        }
        assert(transmit_task == NULL);
        assert(osKernelGetState() == taskSCHEDULER_RUNNING && !xPortIsInsideInterrupt());
        transmit_task = xTaskGetCurrentTaskHandle();
        const uint32_t num_notifs = ulTaskNotifyTake(pdTRUE, 1000);
        UNUSED(num_notifs);
        transmit_task = NULL;
    }
    return hw_utils_convertHalStatus(HAL_FDCAN_AddMessageToTxFifoQ(hcan, &tx_header, msg->data.data8));
}
void hw::fdcan::init() const 
{
    assert(!ready);
    // Configure a single filter bank that accepts any message.
    FDCAN_FilterTypeDef filter;
    filter.IdType       = FDCAN_EXTENDED_ID; // 29 bit ID
    filter.FilterIndex  = 0;
    filter.FilterType   = FDCAN_FILTER_MASK;
    filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    filter.FilterID1    = 0x00000000; // Standard CAN ID bits [28:0]
    filter.FilterID2    = 0x1FFFFFFF; // Mask bits for Extended CAN ID

// Fields only enabled for H7
#if defined(STM32H753xx)
    filter.IsCalibrationMsg = 0;
    filter.RxBufferIndex    = 0;    
#endif

    const ExitCode configure_filter_status = 
        hw_utils_convertHalStatus(HAL_FDCAN_ConfigFilter(hcan, &filter));
    ASSERT_EXIT_OK(configure_filter_status);

    // Configure interrupt mode for CAN peripheral.
    const ExitCode configure_notis_ok = hw_utils_convertHalStatus(HAL_FDCAN_ActivateNotification(
        hfdcan,
        FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_RX_FIFO1_NEW_MESSAGE | FDCAN_IT_BUS_OFF | FDCAN_IT_TX_COMPLETE,
        FDCAN_TX_BUFFER0));
    ASSERT_EXIT_OK(configure_notis_ok);

    // Start the FDCAN peripheral.
    const ExitCode start_status = hw_utils_convertHalStatus(HAL_FDCAN_Start(hcan));
    ASSERT_EXIT_OK(start_status);
    ready = true;
}

void hw::fdcan::deinit() const 
{
    assert(HAL_FDCAN_Stop(hcan) == HAL_OK);
    assert(HAL_FDCAN_DeInit(hcan) == HAL_OK);
}

ExitCode hw::fdcan::fdcan_transmit(const io::CanMsg &msg) 
{

    assert(ready);
    FDCAN_TxHeaderTypeDef tx_header;
    tx_header.Identifier          = msg.std_id;
    tx_header.IdType              = (msg.std_id > MAX_11_BITS_VALUE) ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
    tx_header.TxFrameType         = FDCAN_DATA_FRAME;
    tx_header.DataLength          = msg.dlc << 16; // Data length code needs to be shifted by 16 bits.
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch       = FDCAN_BRS_OFF;
    tx_header.FDFormat            = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker       = 0;
    return tx(tx_header, const_cast<io::fdcanMsg *>(&msg));
}

ExitCode hw::fdcan::fdcan_transmit(const io::CanMsg &msg) 
{
    assert(ready);

    uint32_t dlc;
    if (msg.dlc <= 8)
    {
        dlc = msg.dlc << 16; // Data length code needs to be shifted by 16 bits.
    }
    else if (msg.dlc <= 12)
    {
        dlc = FDCAN_DLC_BYTES_12;
    }
    else if (msg.dlc <= 16)
    {
        dlc = FDCAN_DLC_BYTES_16;
    }
    else if (msg.dlc <= 20)
    {
        dlc = FDCAN_DLC_BYTES_20;
    }
    else if (msg.dlc <= 24)
    {
        dlc = FDCAN_DLC_BYTES_24;
    }
    else if (msg.dlc <= 32)
    {
        dlc = FDCAN_DLC_BYTES_32;
    }
    else if (msg.dlc <= 48)
    {
        dlc = FDCAN_DLC_BYTES_48;
    }
    else
    {
        dlc = FDCAN_DLC_BYTES_64;
    }

    FDCAN_TxHeaderTypeDef tx_header;
    tx_header.Identifier          = msg.std_id;
    tx_header.IdType              = (msg.std_id > MAX_11_BITS_VALUE) ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
    tx_header.TxFrameType         = FDCAN_DATA_FRAME;
    tx_header.DataLength          = dlc; // Data length code needs to be shifted by 16 bits.
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch       = hcan->Init.FrameFormat == FDCAN_FRAME_FD_BRS ? FDCAN_BRS_ON : FDCAN_BRS_OFF;
    tx_header.FDFormat            = FDCAN_FD_CAN;
    tx_header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker       = 0;
    return tx(tx_header, const_cast<io::fdcanMsg *>(&msg));
}

ExitCode hw::fdcan::receive(const uint32_t rx_fifo, io::CanMsg &msg) const 
{
    assert(ready);
    FDCAN_RxHeaderTypeDef header;

    RETURN_IF_ERR(
        hw_utils_convertHalStatus(HAL_FDCAN_GetRxMessage(hcan, rx_fifo, &header, msg.data.data8)););

    // Copy metadata from HAL's CAN message struct into our custom CAN
    // message struct
    msg.std_id    = header.Identifier;
    msg.dlc       = header.DataLength >> 16; // Data length code needs to be un-shifted by 16 bits.
    msg.timestamp = io::time::getCurrentMs();
    msg.bus       = bus_num;

    return ExitCode::EXIT_CODE_OK;
}

CFUNC void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
    LOG_INFO("FDCAN on bus %d detected on error %x", hw::fdcan_getHandle(hfdcan)->getBusNum(), ErrorStatusITs);
    if ((ErrorStatusITs & FDCAN_IT_BUS_OFF) != RESET)
    {
        FDCAN_ProtocolStatusTypeDef protocolStatus;
        HAL_FDCAN_GetProtocolStatus(hfdcan, &protocolStatus);
        if (protocolStatus.BusOff)
        {
            LOG_ERROR("FDCAN on bus %d is in BUS OFF state!", hw::fdcan_getHandle(hfdcan).getBusNum());
        }
    }
}

static ExitCode handleCallback(FDCAN_HandleTypeDef *hfdcan, uint8_t fifo)
{
    const hw::fdcan &handle = hw::fdcan_getHandle(hfdcan);

    io::fdcanMsg rx_msg;

    RETURN_IF_ERR_SILENT(handle.receive(fifo, rx_msg));

    if(handle.receive_callback == nullptr) {
        LOG_ERROR("CAN has no callback configured!");
        return ExitCode::EXIT_CODE_ERROR;
    }

    handle.receive_callback(&rx_msg);
    return ExitCode::EXIT_CODE_OK;
}

CFUNC void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, const uint32_t RxFifo0ITs)
{
    UNUSED(RxFifo0ITs);
    while (IS_EXIT_OK(handleCallback(hfdcan, FDCAN_RX_FIFO0)));
}

CFUNC void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, const uint32_t RxFifo1ITs)
{
    UNUSED(RxFifo1ITs);
    while (IS_EXIT_OK(handleCallback(hfdcan, FDCAN_RX_FIFO1)));
}

CFUNC void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes)
{
    const hw::fdcan &handle = hw::fdcan_getHandle(hfdcan);
    if (handle.transmit_task == NULL)
    {
        return;
    }
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(handle.transmit_task, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}