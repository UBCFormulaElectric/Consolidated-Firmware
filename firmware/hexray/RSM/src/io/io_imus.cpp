#include "io_imu.hpp"
#include "io_imus.hpp"

namespace io::imus
{
imu imu_rear(hw::spi::imu_sd);

result<void> init()
{
    return imu_rear.init();
}
} // namespace io::imus
