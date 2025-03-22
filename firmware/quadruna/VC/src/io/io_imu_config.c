#include "io_imu_config.h"

const imuConfig imu_config = { .imu_i2c_handle = &imu_i2c,
                               .x_accel_offset = 0.0f,
                               .y_accel_offset = 2.6f,
                               .z_accel_offset = -20.0f,
                               .pitch_offset   = 0.0f,
                               .roll_offset    = 1.2f,
                               .yaw_offset     = 0.0f };
