#include "hw_i2c.h"

I2C_HandleTypeDef *const i2c_bus_handles[HW_I2C_BUS_COUNT] = {
    [HW_I2C_BUS_1] = &hi2c1,
    [HW_I2C_BUS_2] = &hi2c2,
};

const I2cDeviceConfig i2c_device_config[HW_I2C_DEVICE_COUNT] = {
    [HW_I2C_DEVICE_IMU]           = { .bus = HW_I2C_BUS_2, .target_address = 0x6B, .timeout_ms = 100 },
    [HW_I2C_DEVICE_POTENTIOMETER] = { .bus = HW_I2C_BUS_1, .target_address = 0x2F, .timeout_ms = 100 },
};
