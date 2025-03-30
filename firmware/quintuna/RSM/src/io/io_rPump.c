#include "hw_i2cs.h"

#define MAX_WIPER_VALUE (256.0f)
#define MIN_WIPER_VALUE (0.0f)

bool io_rPump_isPumpReady(const I2cDevice *device){
    return hw_i2c_isTargetReady(device);
}

void io_rPump_write(const I2cDevice *device, uint8_t data) {
    uint8_t buffer[1];
    buffer[0] = data;
    hw_i2c_memoryWrite(device, 0x02, buffer, sizeof(buffer));
}
 
uint8_t io_rPump_read(const I2cDevice *device) {
    uint8_t buffer[1];
    hw_i2c_memoryRead(device, 0x02, buffer, sizeof(buffer));
    return buffer[0];
}

void io_rPump_setPercentage(const I2cDevice *device, float percentage) { 
    if (percentage > 100.0f) {
        percentage = 100.0f;
    } else if (percentage < 0.0f) {
        percentage = 0.0f;
    }
    uint8_t data = (uint8_t)((1.0f - (percentage / 100.0f)) * MAX_WIPER_VALUE);
    io_rPump_write(device, data);
}

float io_rPump_readPercentage(const I2cDevice *device) {
    uint8_t data = io_rPump_read(device);
    float percentage = (1.0f - ((float)data / MAX_WIPER_VALUE)) * 100.0f;
    return percentage;
}