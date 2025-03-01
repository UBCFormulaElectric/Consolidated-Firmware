#include "main.h"
#include "hw_i2cs.h"

const I2cDevice imu = { .bus = &hi2c1, //fix this after i2c merge
                        .target_address = 0x6B,
                        .timeout_ms = 40 };
