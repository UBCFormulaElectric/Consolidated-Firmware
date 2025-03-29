#include "hw_i2cs.h"

#define VOLTAGE_MAX (5.0f)
#define VOLTAGE_MIN (0.0f)

#define VOLTAGE_CONVERSION_FACTOR (100.0f / (VOLTAGE_MAX - VOLTAGE_MIN))

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

void io_rPump_setPercentage(const I2cDevice *device, float percentage) { 
    if (percentage > 100.0f) {
        percentage = 100.0f;
    } else if (percentage < 0.0f) {
        percentage = 0.0f;
    }
    uint8_t data = (uint8_t)(percentage / VOLTAGE_CONVERSION_FACTOR);
    io_rPump_write(device, data);
}

float io_rPump_readPercentage(const I2cDevice *device) {
    uint8_t data = io_rPump_read(device);
    float percentage = (float)data * VOLTAGE_CONVERSION_FACTOR;
    return percentage;
}