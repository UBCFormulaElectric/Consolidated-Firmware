#include "hw_i2c.h"

// Number of attempts made to check if connected device is ready to communicate.
const int NUM_TRIALS = 5;

namespace hw::i2c
{
bool isTargetReady()
{
    return HAL_I2C_IsDeviceReady(i2c_handle, target_address << 1, (uint32_t)NUM_TRIALS, timeout_ms) == HAL_OK;
}

bool transmit(uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (isTargetReady())
    {
        return HAL_I2C_Master_Transmit(i2c_handle, target_address << 1, tx_buffer, tx_buffer_size, timeout_ms) ==
               HAL_OK;
    }
    return false;
}

bool receive(uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (isTargetReady())
    {
        return HAL_I2C_Master_Receive(
                   i2c_handle, (uint16_t)((target_address << 1) | 0x1), rx_buffer, rx_buffer_size, timeout_ms) ==
               HAL_OK;
    }
    return false;
}

bool memRead(uint16_t mem_addr, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    if (isTargetReady())
    {
        return HAL_I2C_Mem_Read(
                   i2c_handle, (uint16_t)((target_address << 1) | 0x1), mem_addr, I2C_MEMADD_SIZE_8BIT, rx_buffer,
                   rx_buffer_size, timeout_ms) == HAL_OK;
    }
    return false;
}

bool memWrite(uint16_t mem_addr, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    if (isTargetReady())
    {
        return HAL_I2C_Mem_Write(
                   i2c_handle, (target_address << 1), mem_addr, I2C_MEMADD_SIZE_8BIT, tx_buffer, tx_buffer_size,
                   timeout_ms) == HAL_OK;
    }
    return false;
}
} // namespace hw::i2c
