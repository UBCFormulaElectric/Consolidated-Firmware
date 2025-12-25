#include "hw_cans.hpp"
#include "hw_can.hpp"
#include "main.h"
#include "tasks.h"
#include <cassert>
//main.h not linking check cmake?
//is bus number arbitrary?
namespace hw
{
    //no tasks_runCanRxCallback yet in tasks.c (need bootloader stuff)
    can fdcan1(hfdcan1, 0, tasks_runCanRxCallback);
    //should I change type to FDCAN_HandleTypeDef in hw_can.hpp?
    const can &hw_can_getHandle(const FDCAN_HandleTypeDef *hcan)
    {
        assert(hcan == fdcan1.getHcan());     
        return fdcan1;
    }
    
}