#pragma once
#include "app_canUtils.hpp"
#include "torque_allocator.hpp"

namespace app::tv::controllers
{
[[nodiscard]] allocator::OptimizerWeights getOptimizerWeights(app::can_utils::DriveMode drivemode);
} // namespace app::tv::controllers
