#include "io_potentiometer.h"
#include "hw_i2c.h"
#include "hw_hal.h"
#include "hw_i2cs.h"

bool io_potentiometer_init()
{
    return hw_i2c_isTargetReady(&potentiometer_i2c);
}

void io_potentiometer_setWiper(uint8_t position)
{
    uint8_t buffer[1];
    buffer[0] = position;

    // TODO: Handle error on Quintuna!
    hw_i2c_transmit(&potentiometer_i2c, buffer, sizeof(buffer));
}

uint8_t io_potentiometer_readWiper()
{
    uint8_t buffer[1];
    // TODO: Handle error on Quintuna!
    hw_i2c_receive(&potentiometer_i2c, buffer, sizeof(buffer));
    return buffer[0];
}
