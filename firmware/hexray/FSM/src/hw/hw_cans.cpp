#include "hw_cans.hpp"
#include <cassert>
#include "main.h"
#ifndef USE_CHIMERA
#include "io_canQueues.hpp"
#include "io_bootHandler.hpp"
#include "bootloader_fsm.hpp"
#endif

namespace hw::can
{
const fdcan fdcan1{
    hfdcan1,
    [](const CanMsg &msg)

    {
#ifndef USE_CHIMERA
        io::bootHandler::processBootRequest(msg, board_highbits);
        LOG_IF_ERR(can_rx_queue.push({ msg.std_id, msg.dlc, msg.data, true, app::can_utils::BusEnum::Bus_FDCAN }));
#endif
    }
}; // define callback func in io
} // namespace hw::can

namespace hw
{
const fdcan &fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can::fdcan1.getHfdcan());
    return can::fdcan1;
}
} // namespace hw