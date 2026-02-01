#include "app_imu.hpp"
#include "io_imus.hpp"
#include "util_errorCodes.hpp"

extern "C"
{
#include "app_canTx.h"
}

#ifdef TARGET_TEST
io::imu::Imu imu_config;
#endif // TARGET_TEST
namespace app::imu
{
static FSMImuFaults imu_faults = { .accel_x_fault = std::unexpected(ErrorCode::ERROR),
                                   .accel_y_fault = std::unexpected(ErrorCode::ERROR),
                                   .accel_z_fault = std::unexpected(ErrorCode::ERROR),
                                   .gyro_x_fault  = std::unexpected(ErrorCode::ERROR),
                                   .gyro_y_fault  = std::unexpected(ErrorCode::ERROR),
                                   .gyro_z_fault  = std::unexpected(ErrorCode::ERROR) };

void broadcast()
{
#ifdef TARGET_TEST
    float accel_x{ 0 }, accel_y{ 0 }, accel_z{ 0 };
    float gyro_x{ 0 }, gyro_y{ 0 }, gyro_z{ 0 };

    imu_faults.accel_x_fault = imu_config.getAccelX(accel_x);
    imu_faults.accel_y_fault = imu_config.getAccelY(accel_y);
    imu_faults.accel_z_fault = imu_config.getAccelZ(accel_z);
    imu_faults.gyro_x_fault  = imu_config.getGyroX(gyro_x);
    imu_faults.gyro_y_fault  = imu_config.getGyroY(gyro_y);
    imu_faults.gyro_z_fault  = imu_config.getGyroZ(gyro_z);

    accel_x = imu_faults.accel_x_fault ? accel_x : 0.0f;
    accel_y = imu_faults.accel_y_fault ? accel_y : 0.0f;
    accel_z = imu_faults.accel_z_fault ? accel_z : 0.0f;
    gyro_x  = imu_faults.gyro_x_fault ? gyro_x : 0.0f;
    gyro_y  = imu_faults.gyro_y_fault ? gyro_y : 0.0f;
    gyro_z  = imu_faults.gyro_z_fault ? gyro_z : 0.0f;

    io::imu::Imu::AccelData imu_accel_data = { accel_x, accel_y, accel_z };
    io::imu::Imu::GyroData  imu_gyro_data  = { gyro_x, gyro_y, gyro_z };

    app_canTx_FSM_LinearAccelerationInX_set(imu_accel_data.x);
    app_canTx_FSM_LinearAccelerationInY_set(imu_accel_data.y);
    app_canTx_FSM_LinearAccelerationInZ_set(imu_accel_data.z);
    app_canTx_FSM_RollAngularAcceleration_set(imu_gyro_data.x);
    app_canTx_FSM_PitchAngularAcceleration_set(imu_gyro_data.y);
    app_canTx_FSM_YawAngularAcceleration_set(imu_gyro_data.z);
#endif // TARGET_TEST
}
} // namespace app::imu
