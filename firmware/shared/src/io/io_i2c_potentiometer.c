#include "io_i2c_potentiometer.h"
#include "hw_i2c.h"

const uint8_t TARGET_ADDRESS = 0b0101111;
const uint16_t BUFFER_SIZE = 1; // TODO


void set_wiper(int value, I2C_HandleTypeDef i2c_handle) {
    I2cInterface* interface = {
        .i2c_handle = i2c_handle,
        .target_address = TARGET_ADDRESS,
        .timeout_ms = 30
    };

    uint8_t* buffer[BUFFER_SIZE];
    // TODO: Fill buffer

    hw_i2c_transmit(interface, buffer, BUFFER_SIZE);
}