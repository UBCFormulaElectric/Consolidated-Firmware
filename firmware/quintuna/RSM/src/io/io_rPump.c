#include "hw_i2cs.h"

const I2cDevice r_pump = { .src = &r_pump_i2c };

bool io_rPump_isPumpReady(const Pump *pump){
    return hw_i2c_isTargetReady(pump->src);
}

void io_rPump_write(const Pump *pump, uint8_t data) {
    uint8_t buffer[1];
    buffer[0] = data;
    hw_i2c_transmit(pump->src, buffer, sizeof(buffer));
}

uint8_t io_rPump_read(const Pump *pump) {
    uint8_t buffer[1];
    hw_i2c_receive(pump->src, buffer, sizeof(buffer));
    return buffer[0];
}







