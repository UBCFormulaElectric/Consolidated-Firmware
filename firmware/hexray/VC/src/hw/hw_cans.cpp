#include "hw_cans.hpp"
#include "main.h"

namespace hw::cans
{
fdcan fdcan1(hfdcan1, 0, [](const io::CanMsg &msg) { UNUSED(msg); });
fdcan fdcan3(hfdcan3, 0, [](const io::CanMsg &msg) { UNUSED(msg); });
} // namespace hw::cans

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    if (hfdcan == hw::cans::fdcan1.getHfdcan())
    {
        return hw::cans::fdcan1;
    }
    if (hfdcan == hw::cans::fdcan3.getHfdcan())
    {
        return cans::fdcan3;
    }
    // fallback
    return cans::fdcan1;
}
