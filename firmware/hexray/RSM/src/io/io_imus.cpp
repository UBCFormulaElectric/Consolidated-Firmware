#include "hw_spis.hpp"
#include "io_imu.hpp"
#include "io_imus.hpp"

namespace io::imu
{
ImuFilterConfig imu_filter;

Imu imu_config(hw::spi::imu);
} // namespace io::imu
