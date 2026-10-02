#include "io_imu.hpp"
#include "hw_spis.hpp"

namespace io::imus
{
imu imu_front(
    hw::spi::imu_spi,
    ImuFilterConfig{ .enable_gyro_dlpf = true, .gyro_dlpf_cutoff = GyroDlpfConfig::BW_20HZ_NOISE_31HZ });

result<void> init()
{
    return imu_front.init();
}
} // namespace io::imus
