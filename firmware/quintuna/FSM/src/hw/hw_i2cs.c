#include "main.h"
#include "hw_i2cs.h"

const I2cInterface imu = { .i2c_handle = &hi2c1,
                           .target_address = 0x6B,
                           .timeout_ms = 40 };
