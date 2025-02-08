#include "main.h"
#include "hw_i2cs.h"

const I2C imu = { .handle = &i2c1_sda, &i2c1_scl };
