#include "hw_i2c.h"

// Number of attempts made to check if connected device is ready to communicate.
const uint32_t NUM_TRIALS = 5;

bool hw_i2c_transmit(const I2cInterface *i2c, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (hw_i2c_isTargetReady(i2c))
    {
        return HAL_I2C_Master_Transmit(
                   i2c->i2c_handle, i2c->target_address << 1, tx_buffer, tx_buffer_size, i2c->timeout_ms) == HAL_OK;
    }
    return false;
}

bool hw_i2c_receive(const I2cInterface *i2c, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (hw_i2c_isTargetReady(i2c))
    {
        uint16_t address = i2c->target_address << 1;
        address |= 1;
        return HAL_I2C_Master_Receive(i2c->i2c_handle, address, rx_buffer, rx_buffer_size, i2c->timeout_ms) == HAL_OK;
    }
    return false;
}

bool hw_i2c_isTargetReady(const I2cInterface *i2c)
{
    return HAL_I2C_IsDeviceReady(i2c->i2c_handle, i2c->target_address << 1, NUM_TRIALS, i2c->timeout_ms) == HAL_OK;
}