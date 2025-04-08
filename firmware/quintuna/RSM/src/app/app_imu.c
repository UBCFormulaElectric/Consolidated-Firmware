#include "io_imus.h"
#include "app_imu.h"
#include "app_canTx.h"


void app_imu_broadcast(void) {
    float x_lin_accel, y_lin_accel, z_lin_accel;
    float roll_ang_vel, pitch_ang_vel, yaw_ang_vel;
    
    io_imu_getLinAccel(&x_lin_accel, &y_lin_accel, &z_lin_accel);
    io_imu_getAngVel(&roll_ang_vel, &pitch_ang_vel, &yaw_ang_vel);

    app_canTx_RSM_LinearAccelerationInX_set(x_lin_accel);
    app_canTx_RSM_LinearAccelerationInY_set(y_lin_accel);
    app_canTx_RSM_LinearAccelerationInZ_set(z_lin_accel);
    app_canTx_RSM_RollAngularAcceleration_set(roll_ang_vel);
    app_canTx_RSM_PitchAngularAcceleration_set(pitch_ang_vel);
    app_canTx_RSM_YawAngularAcceleration_set(yaw_ang_vel);
}
