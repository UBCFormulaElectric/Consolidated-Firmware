#include "hw_i2cs.h"
#include "main.h"

// TODO: Setup I2Cs.
I2C_HandleTypeDef *i2c_bus_handles[HW_I2C_BUS_COUNT] = { [HW_I2C_3] = &hi2c3 };
