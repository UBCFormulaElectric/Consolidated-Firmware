#pragma once
#include "hw_can.hpp"

namespace hw
{
namespace cans
{
    extern fdcan fdcan1;
}
const fdcan &hw_can_getHandle(const FDCAN_HandleTypeDef *hcan);

} // namespace hw
