#include "hw_cans.hpp"
#include "main.h"

#include <cassert>

namespace hw::can
{
constexpr fdcan can1(hfdcan1, [](const CanMsg &msg) { UNUSED(msg); });
} // namespace hw::can

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::can::can1.getHfdcan());
    return hw::can::can1;
}
