#include "io_i2c_potentiometer.h"
#include "hw_i2c.h"

const uint8_t TARGET_ADDRESS = 0b0101111;

static I2cInterface pot;

bool io_i2c_potentiometer_init(I2C_HandleTypeDef i2c_handle)
{
    pot.i2c_handle = &i2c_handle;
    pot.target_address = TARGET_ADDRESS;
    pot.timeout_ms = 100;

    return hw_i2c_is_target_ready(&pot);
}

void io_i2c_set_wiper(uint8_t position)
{
    uint8_t* buffer[1];
    buffer[0] = position;

    hw_i2c_transmit(&pot, buffer, sizeof(buffer));
}