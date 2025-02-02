#include "main.h"
#include "hw_i2c.h"

const I2cInterface r_pump = { 
    .i2c_handle = &hi2c3,
    .target_address = 0,
    .timeout_ms = 0
};

const I2cInterface imu = { 
    .i2c_handle = &hi2c1,
    .target_address = 0,
    .timeout_ms = 0
};