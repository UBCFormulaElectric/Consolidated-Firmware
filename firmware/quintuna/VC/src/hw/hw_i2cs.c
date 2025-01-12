#include "hw_i2cs.h"
#include "main.h"

const I2cInterface pwr_mon = { &hi2c1, 0b0010000, 30 };