#include "io_imu.hpp"
#include "hw_spis.hpp"

namespace io::imus
{
Imu imu_r(hw::spi::imu_sd);
}
