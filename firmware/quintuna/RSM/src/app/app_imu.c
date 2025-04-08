#include "io_imus.h"
#include "app_imu.h"

void app_imu_broadcast(void) {
    float x_lin_accel, y_lin_accel, z_lin_accel;
    float roll_ang_vel, pitch_ang_vel, yaw_ang_vel;
    
    io_imu_getLinAccel(&x_lin_accel, &y_lin_accel, &z_lin_accel);
    io_imu_getAngVel(&roll_ang_vel, &pitch_ang_vel, &yaw_ang_vel);


}
