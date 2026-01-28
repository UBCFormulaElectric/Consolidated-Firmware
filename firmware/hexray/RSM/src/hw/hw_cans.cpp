#include "hw_cans.hpp"
#include "main.h"
#include "tasks.h"

//  is bus number arbitrary?
// why can i2c be hw::i2c but this cannot be hw::can
namespace hw::cans
{
// no tasks_runCanRxCallback yet in tasks.c (need bootloader stuff)
can fdcan1(hfdcan1, 0, tasks_runCanRxCallback);

} // namespace hw::cans

// should I change type to FDCAN_HandleTypeDef in hw_can.hpp?
const hw::can &hw_can_getHandle(const FDCAN_HandleTypeDef *hcan)
{
    assert(hcan == hw::cans::fdcan1.getHcan());
    return hw::cans::fdcan1;
}