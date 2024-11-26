#include "io_i2cPotentiometer.h"
#include "hw_i2c.h"

const uint8_t TARGET_ADDRESS = 0x2F;

I2c i2c_config(&hi2c1, TARGET_ADDRESS, 100);

bool isReady()
{
    return hw::i2c::i2c_config.isTargetReady();
}

void setWiper(uint8_t position)
{
    uint8_t buffer[1];
    buffer[0] = position;

    hw::i2c::i2c_config.transmit(buffer, sizeof(buffer));
}

uint8_t readWiper()
{
    uint8_t buffer[1];
    hw::i2c::i2c_config.receive(buffer, sizeof(buffer));
    return buffer[0];
}
