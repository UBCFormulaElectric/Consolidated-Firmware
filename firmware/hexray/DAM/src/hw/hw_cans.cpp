#include "hw_cans.hpp"
#include "main.h"
#include <cassert>
#include <cstdint>

#ifndef USE_CHIMERA
#include "io_bootHandler.hpp"
#include "bootloader_DAM.hpp"
#include "io_canQueues.hpp"

namespace
{
[[nodiscard]] uint32_t payloadSizeFromDlc(const uint32_t dlc)
{
    if (dlc <= 8)
    {
        return dlc;
    }
    if (dlc <= 12)
    {
        return (dlc - 6) * 4;
    }
    if (dlc == 13)
    {
        return 32;
    }
    if (dlc == 14)
    {
        return 48;
    }
    if (dlc == 15)
    {
        return 64;
    }
    assert(false);
    return 0;
}
} // namespace
#endif

constexpr hw::fdcan fdcan1(
    hfdcan1,
    [](const hw::CanMsg &msg)
    {
#ifndef USE_CHIMERA
        io::bootHandler::processBootRequest(msg, board_highbits);
        (void)can_rx_queue.push(
            io::CanMsg{ msg.std_id, payloadSizeFromDlc(msg.dlc), msg.data, true, app::can_utils::BusEnum::Bus_FDCAN });
#endif
    });

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan());
    return fdcan1;
}
