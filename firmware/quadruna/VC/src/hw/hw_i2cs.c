#include "hw_i2cs.h"
#include "main.h"

const I2cInterface imu = { .i2c_handle = &hi2c2, .target_address = 0x6B, .timeout_ms = 100 };
