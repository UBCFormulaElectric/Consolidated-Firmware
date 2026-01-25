#pragma once

namespace fakes::io::imu
{
void reset_init();
bool get_init();
void set_AccelX(float x_acceleration);
void set_AccelY(float y_acceleration);
void set_AccelZ(float z_acceleration);
void set_GyroRoll(float roll_velocity);
void set_GyroPitch(float pitch_velocity);
void set_GyroYaw(float yaw_velocity);
} // namespace fakes::io::imu