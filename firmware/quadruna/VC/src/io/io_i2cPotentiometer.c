#include "io_i2cPotentiometer.h"
#include "hw_i2c.h"
#include "hw_hal.h"

const uint8_t TARGET_ADDRESS = 0x2F;

static I2cDevice pot = { .bus = HW_I2C_BUS_1, TARGET_ADDRESS, 100 };

bool io_i2cPotentiometer_Init()
{
    return hw_i2c_isTargetReady(&pot);
}

void io_i2cPotentiometer_setWiper(uint8_t position)
{
    uint8_t buffer[1];
    buffer[0] = position;

    hw_i2c_transmit(&pot, buffer, sizeof(buffer));
}

uint8_t io_i2cPotentiometer_readWiper()
{
    uint8_t buffer[1];
    hw_i2c_receive(&pot, buffer, sizeof(buffer));
    return buffer[0];
}
