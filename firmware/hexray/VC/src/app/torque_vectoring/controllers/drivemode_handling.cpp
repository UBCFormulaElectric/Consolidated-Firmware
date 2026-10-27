#include "drivemode_handling.hpp"
#include "controllers_config.hpp"

namespace app::tv::controllers
{
allocator::OptimizerWeights getOptimizerWeights(const app::can_utils::DriveMode drivemode)
{
    switch (drivemode)
    {
        case app::can_utils::DriveMode::ACCELERATION:
            return allocator::accel_ow;
        case app::can_utils::DriveMode::TRACK:
            return allocator::track_ow;
        case app::can_utils::DriveMode::VANILLA:
        case app::can_utils::DriveMode::COUNT:
        default:
            return allocator::vanilla_ow;
    }
}
} // namespace app::tv::controllers
