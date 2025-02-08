#include "io_i2cPotentiometer.h"
#include "hw_i2c.h"
#include "hw_hal.h"
#include "hw_i2cs.h"

bool io_i2cPotentiometer_Init()
{
    return hw_i2c_isTargetReady(HW_I2C_DEVICE_POTENTIOMETER);
}

void io_i2cPotentiometer_setWiper(uint8_t position)
{
    uint8_t buffer[1];
    buffer[0] = position;

    hw_i2c_transmit(HW_I2C_DEVICE_POTENTIOMETER, buffer, sizeof(buffer));
}

uint8_t io_i2cPotentiometer_readWiper()
{
    uint8_t buffer[1];
    hw_i2c_receive(HW_I2C_DEVICE_POTENTIOMETER, buffer, sizeof(buffer));
    return buffer[0];
}
