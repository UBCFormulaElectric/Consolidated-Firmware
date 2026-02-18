#include "io_imu.hpp"
#include "hw_spis.hpp"

namespace io::imus
{
const io::imu::Imu imu_config(hw::spi::imu_spi);
}
