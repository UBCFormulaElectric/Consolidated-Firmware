#include "io_imus.hpp"
#include "hw_spis.hpp"

namespace io::imus
{
imu IMU1(
    imu1,
    ImuFilterConfig{ .enable_accel_dlpf = true,
                     .enable_gyro_dlpf  = true,
                     .accel_dlpf_cutoff = AccelDlpfConfig::BW_10HZ_NOISE_16HZ,
                     .gyro_dlpf_cutoff  = GyroDlpfConfig::BW_10HZ_NOISE_16HZ });
imu IMU2(imu2, ImuFilterConfig{ .enable_gyro_dlpf = true, .gyro_dlpf_cutoff = GyroDlpfConfig::BW_20HZ_NOISE_31HZ });
imu IMU3(imu3, ImuFilterConfig{ .enable_gyro_dlpf = true, .gyro_dlpf_cutoff = GyroDlpfConfig::BW_20HZ_NOISE_31HZ });

result<void> init()
{
    LOG_IF_ERR(IMU1.init());
    LOG_IF_ERR(IMU2.init());
    LOG_IF_ERR(IMU3.init());
    return {};
}
} // namespace io::imus