#include "drivemode_handling.hpp"
#include "controllers_config.hpp"

namespace app::tv::controllers
{
allocator::OptimizerWeights getOptimizerWeights(const shared_datatypes::DriveMode drivemode)
{
    switch (drivemode)
    {
        case shared_datatypes::DriveMode::ACCELERATION:
            return allocator::accel_ow;
        case shared_datatypes::DriveMode::TRACK:
            return allocator::track_ow;
        case shared_datatypes::DriveMode::VANILLA:
        default:
            return allocator::vanilla_ow;
    }
}
} // namespace app::tv::controllers
