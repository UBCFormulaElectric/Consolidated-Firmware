#include "io/imu.h"
#include "hw/i2cs.h"

// TODO: set offset values
const imuConfig imu_config = { .imu_i2c_handle = &imu_i2c,
                               .x_accel_offset = 0.0f,
                               .y_accel_offset = 0.0f,
                               .z_accel_offset = 0.0f,
                               .roll_offset    = 0.0f,
                               .pitch_offset   = 0.0f,
                               .yaw_offset     = 0.0f };
