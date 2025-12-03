#include "app_imu.hpp"
#include "io_imus.hpp"
#include "util_errorCodes.hpp"
#include "io_imu.hpp"

extern "C"
{
#include "app_canTx.h"
}

static RSMImuFaults imu_faults = { .x_lin_accel_fault   = ExitCode::EXIT_CODE_ERROR,
                                   .y_lin_accel_fault   = ExitCode::EXIT_CODE_ERROR,
                                   .z_lin_accel_fault   = ExitCode::EXIT_CODE_ERROR,
                                   .roll_ang_vel_fault  = ExitCode::EXIT_CODE_ERROR,
                                   .pitch_ang_vel_fault = ExitCode::EXIT_CODE_ERROR,
                                   .yaw_ang_vel_fault   = ExitCode::EXIT_CODE_ERROR };

namespace app::imu
{
void broadcast()
{
    float x_lin_accel, y_lin_accel, z_lin_accel;
    float roll_ang_vel, pitch_ang_vel, yaw_ang_vel;

    imu_faults.x_lin_accel_fault   = io::imu::getLinearAccelerationX(&x_lin_accel);
    imu_faults.y_lin_accel_fault   = io::imu::getLinearAccelerationY(&y_lin_accel);
    imu_faults.z_lin_accel_fault   = io::imu::getLinearAccelerationZ(&z_lin_accel);
    imu_faults.roll_ang_vel_fault  = io::imu::getAngularVelocityRoll(&roll_ang_vel);
    imu_faults.pitch_ang_vel_fault = io::imu::getAngularVelocityPitch(&pitch_ang_vel);
    imu_faults.yaw_ang_vel_fault   = io::imu::getAngularVelocityYaw(&yaw_ang_vel);

    x_lin_accel   = (bool)imu_faults.x_lin_accel_fault ? 0.0f : x_lin_accel;
    y_lin_accel   = (bool)imu_faults.y_lin_accel_fault ? 0.0f : y_lin_accel;
    z_lin_accel   = (bool)imu_faults.z_lin_accel_fault ? 0.0f : z_lin_accel;
    roll_ang_vel  = (bool)imu_faults.roll_ang_vel_fault ? 0.0f : roll_ang_vel;
    pitch_ang_vel = (bool)imu_faults.pitch_ang_vel_fault ? 0.0f : pitch_ang_vel;
    yaw_ang_vel   = (bool)imu_faults.yaw_ang_vel_fault ? 0.0f : yaw_ang_vel;

    app_canTx_RSM_LinearAccelerationInX_set(x_lin_accel);
    app_canTx_RSM_LinearAccelerationInY_set(y_lin_accel);
    app_canTx_RSM_LinearAccelerationInZ_set(z_lin_accel);
    app_canTx_RSM_RollAngularAcceleration_set(roll_ang_vel);
    app_canTx_RSM_PitchAngularAcceleration_set(pitch_ang_vel);
    app_canTx_RSM_YawAngularAcceleration_set(yaw_ang_vel);
}
} // namespace app::imu
