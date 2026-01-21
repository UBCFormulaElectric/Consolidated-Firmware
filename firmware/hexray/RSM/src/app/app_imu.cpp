#include "app_imu.hpp"
#include "io_imus_new.hpp"
#include "util_errorCodes.hpp"
#include "io_imu.hpp"

extern "C"
{
#include "app_canTx.h"
}

namespace app::imu
{
static RSMImuFaults imu_faults = { .accel_x_fault   = ExitCode::EXIT_CODE_ERROR,
                                   .accel_y_fault   = ExitCode::EXIT_CODE_ERROR,
                                   .accel_z_fault   = ExitCode::EXIT_CODE_ERROR,
                                   .gyro_x_fault    = ExitCode::EXIT_CODE_ERROR,
                                   .gyro_y_fault    = ExitCode::EXIT_CODE_ERROR,
                                   .gyro_z_fault    = ExitCode::EXIT_CODE_ERROR };
    
void broadcast()
{   
    io::imu::Imu imu_config; 

    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;

    imu_faults.accel_x_fault   = io::imu::imu_config.getAccelX(&accel_x);
    imu_faults.accel_y_fault   = io::imu::imu_config.getAccelY(&accel_x);
    imu_faults.accel_z_fault   = io::imu::imu_config.getAccelZ(&accel_x);
    imu_faults.accel_x_fault  = io::imu::imu_config.getGyroX(&gyro_x);
    imu_faults.accel_y_fault  = io::imu::imu_config.getGyroY(&gyro_y);
    imu_faults.accel_z_fault  = io::imu::imu_config.getGyroZ(&gyro_z);

    accel_x   = (bool)imu_faults.accel_x_fault ? 0.0f : accel_x;
    accel_y   = (bool)imu_faults.accel_y_fault ? 0.0f : accel_y;
    accel_z   = (bool)imu_faults.accel_z_fault ? 0.0f : accel_z;
    gyro_x  = (bool)imu_faults.gyro_x_fault ? 0.0f : gyro_x;
    gyro_y = (bool)imu_faults.gyro_y_fault ? 0.0f : gyro_y;
    gyro_z   = (bool)imu_faults.gyro_z_fault ? 0.0f : gyro_z;

    io::imu::Imu::AccelData imu_accel_data = {accel_x, accel_y, accel_z};
    io::imu::Imu::GyroData imu_gyro_data = {gyro_x, gyro_y, gyro_z};

    app_canTx_RSM_LinearAccelerationInX_set(accel_x);
    app_canTx_RSM_LinearAccelerationInY_set(accel_y);
    app_canTx_RSM_LinearAccelerationInZ_set(accel_z);
    app_canTx_RSM_RollAngularAcceleration_set(gyro_x);
    app_canTx_RSM_PitchAngularAcceleration_set(gyro_y);
    app_canTx_RSM_YawAngularAcceleration_set(gyro_z);
}
} // namespace app::imu
