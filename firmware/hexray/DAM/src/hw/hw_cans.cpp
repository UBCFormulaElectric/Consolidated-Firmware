#include "hw_cans.hpp"
#include "main.h"
#include <cassert>

constexpr hw::fdcan can1(hfdcan1, 0, [](const io::CanMsg &msg) { UNUSED(msg); });
const hw::fdcan    &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.getHfdcan());
    return can1;
}
