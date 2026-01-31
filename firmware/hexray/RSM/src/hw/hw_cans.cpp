#include "hw_cans.hpp"
#include "main.h"

//  is bus number arbitrary?
// why can i2c be hw::i2c but this cannot be hw::can
namespace hw::cans
{
// no tasks_runCanRxCallback yet in tasks.c (need bootloader stuff)
fdcan fdcan1(hfdcan1, 0, [](const io::CanMsg &msg) { UNUSED(msg); });
} // namespace hw::cans

// should I change type to FDCAN_HandleTypeDef in hw_can.hpp?
const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    return cans::fdcan1;
}
