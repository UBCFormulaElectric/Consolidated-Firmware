#include "io_i2c_potentiometer.h"
#include "hw_i2c.h"

const uint8_t TARGET_ADDRESS = 0x2F;

extern I2C_HandleTypeDef hi2c1;

static I2cInterface pot = { &hi2c1, TARGET_ADDRESS, 100 };

bool io_i2c_potentiometerInit()
{
    return hw_i2c_isTargetReady(&pot);
}

void io_i2c_setWiper(uint8_t position)
{
    uint8_t buffer[1];
    buffer[0] = position;

    hw_i2c_transmit(&pot, buffer, sizeof(buffer));
}

uint8_t io_i2c_readWiper()
{
    uint8_t buffer[1];
    hw_i2c_receive(&pot, buffer, sizeof(buffer));
    return buffer[0];
}