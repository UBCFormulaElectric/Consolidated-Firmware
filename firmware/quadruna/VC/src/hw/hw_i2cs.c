#include "hw_i2c.h"

I2C_HandleTypeDef *i2c_bus_handles[HW_I2C_BUS_COUNT] = {
    [HW_I2C_BUS_1] = &hi2c1,
    [HW_I2C_BUS_2] = &hi2c2,
};
