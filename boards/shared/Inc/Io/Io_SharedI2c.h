#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "App_SharedExitCode.h"
#include "Io_Hal.h"
#include IO_HAL_I2C

typedef struct
{
    I2C_HandleTypeDef *handle;
    uint32_t           timeout_ms;
} SharedI2c;

void Io_SharedI2c_Init(SharedI2c *i2c, I2C_HandleTypeDef *hal_handle, uint32_t timeout_ms);

ExitCode Io_SharedI2c_Transmit(SharedI2c *i2c, uint8_t address, uint8_t *tx_buf, uint8_t tx_buf_size);

ExitCode Io_SharedI2c_Receive(SharedI2c *i2c, uint8_t address, uint8_t *rx_buf, uint8_t rx_buf_size);

ExitCode Io_SharedI2c_TransmitAndReceive(
    SharedI2c *i2c,
    uint8_t    address,
    uint8_t *  tx_buf,
    uint8_t    tx_buf_size,
    uint8_t *  rx_buf,
    uint8_t    rx_buf_size);
