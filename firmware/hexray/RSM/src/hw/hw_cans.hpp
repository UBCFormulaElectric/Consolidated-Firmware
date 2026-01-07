#pragma once
#include "hw_can.hpp"

namespace hw
{
    namespace cans
    {
        extern can fdcan1;
    }
    const can &hw_can_getHandle(const FDCAN_HandleTypeDef *hcan);
// do I need to include the function or no?
// const can &hw_can_getHandle(const FDCAN_HandleTypeDef *hcan);
} // namespace hw
