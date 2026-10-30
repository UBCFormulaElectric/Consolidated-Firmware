#pragma once
#include "torque_allocator.hpp"
#include "torque_vectoring/shared_datatypes/drive_mode.hpp"

namespace app::tv::controllers
{
[[nodiscard]] allocator::OptimizerWeights getOptimizerWeights(shared_datatypes::DriveMode drivemode);
} // namespace app::tv::controllers
