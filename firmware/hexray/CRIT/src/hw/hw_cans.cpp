#include <hw_cans.hpp>
#include "main.h"

namespace hw::cans
{
const fdcan fdcan1(hfdcan1, io::can_tx::BusEnum::Bus_FDCAN, [](const io::CanMsg &msg) { UNUSED(msg); });
} // namespace hw::cans

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    return hw::cans::fdcan1;
}
