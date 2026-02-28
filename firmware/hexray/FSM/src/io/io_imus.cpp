#include "io_imu.hpp"
#include "hw_spis.hpp"

namespace io::imus
{
<<<<<<< HEAD
const io::imu::Imu imu_config(hw::spi::imu_spi);
=======
Imu imu_front(hw::spi::imu_spi);

std::expected<void, ErrorCode> init()
{
    return imu_front.init();
>>>>>>> master
}
} // namespace io::imus
