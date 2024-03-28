#include "hw_i2c.h"

// Number of attempts made to check if connected device is ready to communicate.
const int NUM_TRIALS = 5;

bool hw_i2c_is_target_ready(const I2cInterface *i2c)
{
    return HAL_I2C_IsDeviceReady(i2c->i2c_handle, i2c->target_address << 1, (uint32_t)NUM_TRIALS, i2c->timeout_ms) ==
           HAL_OK;
}

bool hw_i2c_transmit(const I2cInterface *i2c, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (hw_i2c_is_target_ready(i2c))
    {
        return HAL_I2C_Master_Transmit(
                   i2c->i2c_handle, i2c->target_address << 1, tx_buffer, tx_buffer_size, i2c->timeout_ms) == HAL_OK;
    }
    return false;
}

bool hw_i2c_receive(const I2cInterface *i2c, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (hw_i2c_is_target_ready(i2c))
    {
        return HAL_I2C_Master_Receive(
                   i2c->i2c_handle, (uint16_t)((i2c->target_address << 1) | 0x1), rx_buffer, rx_buffer_size,
                   i2c->timeout_ms) == HAL_OK;
    }
    return false;
}

bool hw_i2c_mem_read(
    const I2cInterface *i2c,
    uint16_t            mem_addr,
    uint16_t            mem_size,
    uint8_t            *rx_buffer,
    uint16_t            rx_buffer_size)
{
    if (hw_i2c_is_target_ready(i2c))
    {
        return HAL_I2C_Mem_Read(
                   i2c->i2c_handle, (uint16_t)((i2c->target_address << 1) | 0x1), mem_addr, mem_size, rx_buffer,
                   rx_buffer_size, i2c->timeout_ms) == HAL_OK;
    }
    return false;
}

bool hw_i2c_mem_write(
    const I2cInterface *i2c,
    uint16_t            mem_addr,
    uint16_t            mem_size,
    uint8_t            *tx_buffer,
    uint16_t            tx_buffer_size)
{
    if (hw_i2c_is_target_ready(i2c))
    {
        return HAL_I2C_Mem_Write(
                   i2c->i2c_handle, (i2c->target_address << 1), mem_addr, mem_size, tx_buffer, tx_buffer_size,
                   i2c->timeout_ms) == HAL_OK;
    }
    return false;
}
