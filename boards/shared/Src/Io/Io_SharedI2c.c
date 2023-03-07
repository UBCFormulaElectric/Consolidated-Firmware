#include "Io_SharedI2c.h"

void Io_SharedI2c_Init(SharedI2c *i2c, I2C_HandleTypeDef *hal_handle, uint32_t timeout_ms)
{
    i2c->handle     = hal_handle;
    i2c->timeout_ms = timeout_ms;
}

ExitCode Io_SharedI2c_Transmit(SharedI2c *i2c, uint8_t address, uint8_t *tx_buf, uint8_t tx_buf_size)
{
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(i2c->handle, address, tx_buf, tx_buf_size, i2c->timeout_ms);

    switch (status)
    {
        case HAL_OK:
        {
            return EXIT_CODE_OK;
        }
        case HAL_TIMEOUT:
        {
            return EXIT_CODE_TIMEOUT;
        }
        default:
        {
            return EXIT_CODE_ERROR;
        }
    }
}

ExitCode Io_SharedI2c_Receive(SharedI2c *i2c, uint8_t address, uint8_t *rx_buf, uint8_t rx_buf_size)
{
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(i2c->handle, address, rx_buf, rx_buf_size, i2c->timeout_ms);

    switch (status)
    {
        case HAL_OK:
        {
            return EXIT_CODE_OK;
        }
        case HAL_TIMEOUT:
        {
            return EXIT_CODE_TIMEOUT;
        }
        default:
        {
            return EXIT_CODE_ERROR;
        }
    }
}

ExitCode Io_SharedI2c_TransmitAndReceive(
    SharedI2c *i2c,
    uint8_t    address,
    uint8_t *  tx_buf,
    uint8_t    tx_buf_size,
    uint8_t *  rx_buf,
    uint8_t    rx_buf_size)
{
    HAL_StatusTypeDef tx_status = Io_SharedI2c_Transmit(i2c, address, tx_buf, tx_buf_size);

    if (tx_status != HAL_OK)
    {
        return tx_status;
    }

    return Io_SharedI2c_Receive(i2c, address, rx_buf, rx_buf_size);
}