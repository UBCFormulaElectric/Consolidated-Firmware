#include "app_imu.hpp"
#include "io_imus.hpp"
#include "util_errorCodes.hpp"

extern "C"
{
#include "app_canTx.h"
}

io::imu::Imu imu_config;

namespace app::imu
{
static RSMImuFaults imu_faults = { .accel_x_fault = ExitCode::EXIT_CODE_ERROR,
                                   .accel_y_fault = ExitCode::EXIT_CODE_ERROR,
                                   .accel_z_fault = ExitCode::EXIT_CODE_ERROR,
                                   .gyro_x_fault  = ExitCode::EXIT_CODE_ERROR,
                                   .gyro_y_fault  = ExitCode::EXIT_CODE_ERROR,
                                   .gyro_z_fault  = ExitCode::EXIT_CODE_ERROR };

void broadcast()
{
    float accel_x{ 0 }, accel_y{ 0 }, accel_z{ 0 };
    float gyro_x{ 0 }, gyro_y{ 0 }, gyro_z{ 0 };

    imu_faults.accel_x_fault = imu_config.getAccelX(accel_x);
    imu_faults.accel_y_fault = imu_config.getAccelY(accel_y);
    imu_faults.accel_z_fault = imu_config.getAccelZ(accel_z);
    imu_faults.gyro_x_fault  = imu_config.getGyroX(gyro_x);
    imu_faults.gyro_y_fault  = imu_config.getGyroY(gyro_y);
    imu_faults.gyro_z_fault  = imu_config.getGyroZ(gyro_z);

    accel_x = (bool)imu_faults.accel_x_fault ? 0.0f : accel_x;
    accel_y = (bool)imu_faults.accel_y_fault ? 0.0f : accel_y;
    accel_z = (bool)imu_faults.accel_z_fault ? 0.0f : accel_z;
    gyro_x  = (bool)imu_faults.gyro_x_fault ? 0.0f : gyro_x;
    gyro_y  = (bool)imu_faults.gyro_y_fault ? 0.0f : gyro_y;
    gyro_z  = (bool)imu_faults.gyro_z_fault ? 0.0f : gyro_z;

    io::imu::Imu::AccelData imu_accel_data = { accel_x, accel_y, accel_z };
    io::imu::Imu::GyroData  imu_gyro_data  = { gyro_x, gyro_y, gyro_z };

    app_canTx_RSM_LinearAccelerationInX_set(imu_accel_data.x);
    app_canTx_RSM_LinearAccelerationInY_set(imu_accel_data.y);
    app_canTx_RSM_LinearAccelerationInZ_set(imu_accel_data.z);
    app_canTx_RSM_RollAngularAcceleration_set(imu_gyro_data.x);
    app_canTx_RSM_PitchAngularAcceleration_set(imu_gyro_data.y);
    app_canTx_RSM_YawAngularAcceleration_set(imu_gyro_data.z);
}
} // namespace app::imu
