#include "io_imu.hpp"
#include "hw_spis.hpp"

namespace io::imus
{
Imu imu_front(hw::spi::imu_spi);

std::expected<void, ErrorCode> init()
{
    return imu_front.init();
}
} // namespace io::imus
