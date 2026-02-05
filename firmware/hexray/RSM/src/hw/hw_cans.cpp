#include "hw_cans.hpp"
#include "main.h"

namespace hw::cans
{
const fdcan fdcan1(hfdcan1, 0, [](const io::CanMsg &msg) { UNUSED(msg); });
} // namespace hw::cans

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    return hw::cans::fdcan1;
}
