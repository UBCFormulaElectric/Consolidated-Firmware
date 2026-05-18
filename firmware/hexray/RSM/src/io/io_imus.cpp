#include "io_imu.hpp"
#include "io_imus.hpp"

namespace io::imus
{
imu imu_rear(hw::spi::imu_sd, ImuFilterConfig{ .enable_gyro_dlpf = true, .gyro_dlpf_cutoff = GyroDlpfConfig::BW_20HZ_NOISE_31HZ });

std::expected<void, ErrorCode> init()
{
    return imu_rear.init();
}
} // namespace io::imus
