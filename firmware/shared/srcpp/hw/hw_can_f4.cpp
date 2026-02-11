#include "hw_can.hpp"
#include <stm32f4xx_hal_can.h>

#undef NDEBUG // TODO remove this in favour of always_assert (we would write this)
#include <assert.h>
#include <FreeRTOS.h>
#include <task.h>

#include "hw_utils.hpp"
#include "io_log.hpp"
#include "io_time.hpp"

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
static consteval uint32_t
    INIT_MASKMODE_16BIT_FiRx(const uint32_t std_id, const uint32_t rtr, const uint32_t ide, const uint32_t ext_id)
{
    return (std_id << 5U & 0xFFE0) | (rtr << 4U & 0x0010) | (ide << 3U & 0x0008) | (ext_id << 0U & 0x0007);
}
// Open CAN filter that accepts any CAN message as long as it uses Standard CAN
// ID and is a data frame.
static constexpr uint32_t MASKMODE_16_BIT_ID_OPEN   = INIT_MASKMODE_16BIT_FiRx(0x0, CAN_RTR_DATA, CAN_ID_STD, 0);
static constexpr uint32_t MASKMODE_16_BIT_MASK_OPEN = INIT_MASKMODE_16BIT_FiRx(0x0, 0x1, 0x1, 0x0);

namespace hw
{

std::expected<void, ErrorCode> can::tx(CAN_TxHeaderTypeDef &tx_header, io::CanMsg &msg)
{
    // Spin until a TX mailbox becomes available.
    for (uint32_t poll_count = 0; HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0U;)
    {
        // the polling is here because if the CAN mailbox is temporarily blocked, we don't want to incur the overhead of
        // context switching
        if (poll_count <= 1000)
        {
            poll_count++;
            continue;
        }
        assert(transmit_task == nullptr);
        assert(osKernelGetState() == taskSCHEDULER_RUNNING && !xPortIsInsideInterrupt());
        transmit_task = xTaskGetCurrentTaskHandle();
        // timeout just in case the tx complete interrupt does not fire properly?
        const uint32_t num_notifs = ulTaskNotifyTake(pdTRUE, 1000);
        UNUSED(num_notifs);
        transmit_task = nullptr;
    }

    // Indicates the mailbox used for transmission, not currently used.
    uint32_t mailbox = 0;
    return hw_utils_convertHalStatus(HAL_CAN_AddTxMessage(hcan, &tx_header, msg.data.data8, &mailbox));
}

void can::init() const
{
    assert(!ready);
    // Configure a single filter bank that accepts any message.
    CAN_FilterTypeDef filter;
    filter.FilterMode       = CAN_FILTERMODE_IDMASK;
    filter.FilterScale      = CAN_FILTERSCALE_32BIT;
    filter.FilterActivation = CAN_FILTER_ENABLE;
    // low and high
    filter.FilterIdHigh     = 0x0000;
    filter.FilterIdLow      = 0x0000;
    filter.FilterMaskIdHigh = 0x0000;
    filter.FilterMaskIdLow  = 0x0000;
    // FIFO assignment
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterBank           = 0;
    filter.SlaveStartFilterBank = 0;

    // Configure and initialize hardware filter.
    const HAL_StatusTypeDef config_filter_status = HAL_CAN_ConfigFilter(hcan, &filter);
    assert(config_filter_status == HAL_OK);

    // Configure interrupt mode for CAN peripheral.
    const HAL_StatusTypeDef interrupt_status =
        HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_BUSOFF);
    assert(interrupt_status == HAL_OK);

    // Start the CAN peripheral.
    assert(HAL_CAN_Start(hcan) == HAL_OK);
    ready = true;
}

void can::deinit() const
{
    assert(HAL_CAN_Stop(hcan) == HAL_OK);
    assert(HAL_CAN_DeInit(hcan) == HAL_OK);
}

// NOTE this design assumes that there is only one task calling this function

std::expected<void, ErrorCode> can::can_transmit(const io::CanMsg &msg)
{
    assert(ready);
    CAN_TxHeaderTypeDef tx_header;

    tx_header.DLC = msg.dlc;

    const bool is_std = msg.std_id <= 0x7FF;
    tx_header.StdId   = is_std ? msg.std_id : 0x0;
    tx_header.ExtId   = !is_std ? msg.std_id : 0x0;
    tx_header.IDE     = is_std ? CAN_ID_STD : CAN_ID_EXT;

    // This field can be either Data Frame or Remote Frame. For our
    // purpose, we only ever transmit Data Frames.
    tx_header.RTR = CAN_RTR_DATA;

    // Enabling this gives us a tick-based timestamp which we do not need. Plus,
    // it would take up 2 bytes of the CAN payload. So we disable the timestamp.
    tx_header.TransmitGlobalTime = DISABLE;

    return tx(tx_header, const_cast<io::CanMsg &>(msg));
}

std::expected<void, ErrorCode> can::receive(const uint32_t rx_fifo, io::CanMsg &msg) const
{
    assert(ready);
    CAN_RxHeaderTypeDef header;

    RETURN_IF_ERR(hw_utils_convertHalStatus(HAL_CAN_GetRxMessage(hcan, rx_fifo, &header, msg.data.data8)));

    // Copy metadata from HAL's CAN message struct into our custom CAN
    // message struct
    switch (header.IDE)
    {
        case CAN_ID_STD:
            msg.std_id = header.StdId;
            break;
        case CAN_ID_EXT:
            msg.std_id = header.ExtId;
            break;
        default:
            assert(false);
    }
    msg.dlc       = header.DLC;
    msg.timestamp = io::time::getCurrentMs();

    return {};
}
} // namespace hw

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void handle_callback(CAN_HandleTypeDef *hcan)
{
    const hw::can &handle = hw::can_getHandle(hcan);

    io::CanMsg rx_msg{};
    // if (IS_EXIT_ERR(hw_can_receive(handle, CAN_RX_FIFO0, &rx_msg)))
    if (not handle.receive(CAN_RX_FIFO0, rx_msg).has_value())
        // Early return if RX msg is unavailable.
        return;
    handle.receive_callback(rx_msg);
}

CFUNC void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    handle_callback(hcan);
}

CFUNC void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    handle_callback(hcan);
}

static void mailbox_complete_handler(CAN_HandleTypeDef *hcan)
{
    const hw::can &can = hw::can_getHandle(hcan);
    if (can.transmit_task == nullptr)
    {
        return;
    }
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(can.transmit_task, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}
CFUNC void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    mailbox_complete_handler(hcan);
}
CFUNC void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    mailbox_complete_handler(hcan);
}
CFUNC void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    mailbox_complete_handler(hcan);
}