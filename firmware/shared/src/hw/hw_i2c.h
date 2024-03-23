#pragma once

#include "hw_hal.h"
#include <stdbool.h>

/**
 * @param i2c_handle The given I2C handle structure.
 * @param target_address The device's 7 bit address value in the datasheet.
 * @param timeout_ms Timout duration.
*/
typedef struct
{
    I2C_HandleTypeDef* i2c_handle;
    uint16_t           target_address;
    uint32_t           timeout_ms;
} I2cInterface;

/**
 * Transmit data to the device connected to the given I2C interface.
 * @param i2c The given I2C interface.
 * @param tx_buffer A pointer to the data buffer containing the data transmitted
 * to the device connected to the I2C interface.
 * @param tx_buffer_size The size of the tx_data buffer.
 * @return True if data is transmitted successfully. Else return false.
 */
bool hw_i2c_transmit(const I2cInterface *i2c, uint8_t *tx_buffer, uint16_t tx_buffer_size);

/**
 * Receive data from the device connected to the given I2C interface.
 * @param i2c The given I2C interface.
 * @param rx_buffer A pointer to the data buffer containing the data transmitted
 * from the device connected to the I2C interface.
 * @param rx_buffer_size The size of the rx_data buffer.
 * @return True if data is transmitted successfully. Else return false.
 */
bool hw_i2c_receive(const I2cInterface *i2c, uint8_t *rx_buffer, uint16_t rx_buffer_size);

/**
 * Check if device connected to the given I2C interface is ready for communication.
 * @param i2c The given I2C interface.
 * @return True if connected device is ready to communicate over I2C.
 */
bool hw_i2c_is_target_ready(const I2cInterface *i2c);