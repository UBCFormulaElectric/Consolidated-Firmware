#include "io_imu.hpp"
#include "io_imus.hpp"

namespace io::imus
{
Imu imu_rear(hw::spi::imu_sd);

std::expected<void, ErrorCode> init()
{
    return imu_rear.init();
}
} // namespace io::imus
