#include "io_imu.hpp"
#include "hw_spis.hpp"

namespace io::imus
{
imu imu_front(hw::spi::imu_spi);

result<void> init()
{
    return imu_front.init();
}
} // namespace io::imus
