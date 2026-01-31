#include "hw_cans.hpp"
#include <cassert>
#include "tasks.h"
#include "main.h"

namespace hw::cans
{
fdcan fdcan1 = fdcan(hfdcan1, 0, [](const io::CanMsg &msg) { UNUSED(msg); }); // define callback func in io
} // namespace hw::cans

namespace hw
{
const fdcan &fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    return hw::cans::fdcan1;
}
} // namespace hw