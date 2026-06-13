#include "io_imus.hpp"
#include "hw_spis.hpp"

<<<<<<< Updated upstream io::imu IMU1(imu1);
io::imu   IMU2(imu2);
io::imu   IMU3(imu3);
== == == = namespace io::imus
{
    // IMU1 gyro DLPF disabled: gyro Z is low-pass filtered downstream in software (see app_imu.cpp).
    // NB: the internal DLPF is gyro-wide, so this also leaves IMU1 gyro X/Y unfiltered.
    imu IMU1(imu1, ImuFilterConfig{});
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
>>>>>>> Stashed changes
