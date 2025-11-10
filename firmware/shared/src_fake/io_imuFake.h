#pragma once

namespace fakes::io::imu
{
void reset_init();
bool get_init();
void set_LinearAccelerationX(float x_acceleration);
void set_LinearAccelerationY(float y_acceleration);
void set_LinearAccelerationZ(float z_acceleration);
void set_AngularVelocityRoll(float roll_velocity);
void set_AngularVelocityPitch(float pitch_velocity);
void set_AngularVelocityYaw(float yaw_velocity);
} // namespace fakes::io_imu
