#include "hw_i2cs.h"

bool io_rPump_isPumpReady(const I2cDevice *device){
    return hw_i2c_isTargetReady(device);
}

void io_rPump_write(const I2cDevice *device, uint8_t data) {
    uint8_t buffer[1];
    buffer[0] = data;
    hw_i2c_transmit(device, buffer, sizeof(buffer));
}

uint8_t io_rPump_read(const I2cDevice *device) {
    uint8_t buffer[1];
    hw_i2c_receive(device, buffer, sizeof(buffer));
    return buffer[0];
}