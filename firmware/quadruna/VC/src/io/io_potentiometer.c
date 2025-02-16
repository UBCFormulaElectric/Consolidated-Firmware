#include "io_potentiometer.h"
#include "hw_i2c.h"
#include "hw_hal.h"
#include "hw_i2cs.h"

bool io_potentiometer_init()
{
    return hw_i2c_isTargetReady(HW_I2C_DEVICE_POTENTIOMETER);
}

void io_potentiometer_setWiper(uint8_t position)
{
    uint8_t buffer[1];
    buffer[0] = position;

    hw_i2c_transmit(HW_I2C_DEVICE_POTENTIOMETER, buffer, sizeof(buffer));
}

uint8_t io_potentiometer_readWiper()
{
    uint8_t buffer[1];
    hw_i2c_receive(HW_I2C_DEVICE_POTENTIOMETER, buffer, sizeof(buffer));
    return buffer[0];
}
