#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "app_utils.h"
#include "main.h"

typedef struct
{
    I2C_HandleTypeDef *const handle;
    TaskHandle_t             task_in_progress;
} I2cBus;

typedef struct
{
    I2cBus  *bus;
    uint8_t  target_address;
    uint32_t timeout_ms;
} I2cDevice;

// Board-specific config: You need to define this function which maps STM32 handle to I2cBus struct!
I2cBus *hw_i2c_getBusFromHandle(const I2C_HandleTypeDef *handle);

/**
 * @brief Check if device connected to the given I2C interface is ready for communication.
 * @param device The given I2C interface.
 * @return True if connected device is ready to communicate over I2C.
 */
ExitCode hw_i2c_isTargetReady(const I2cDevice *device);

/**
 * @brief Receive data from the device connected to the given I2C interface.
 * @param device The given I2C interface.
 * @param rx_buffer A pointer to the data buffer containing the data transmitted
 * from the device connected to the I2C interface.
 * @param rx_buffer_size The size of the rx_data buffer.
 * @return True if data is transmitted successfully. Else return false.
 */
ExitCode hw_i2c_receive(const I2cDevice *device, uint8_t *rx_buffer, uint16_t rx_buffer_size);

/**
 * @brief Transmit data to the device connected to the given I2C interface.
 * @param device The given I2C interface.
 * @param tx_buffer A pointer to the data buffer containing the data transmitted
 * to the device connected to the I2C interface.
 * @param tx_buffer_size The size of the tx_data buffer.
 * @return True if data is transmitted successfully. Else return false.
 */
ExitCode hw_i2c_transmit(const I2cDevice *device, const uint8_t *tx_buffer, uint16_t tx_buffer_size);

/**
 * @brief Read an amount of data from a specific memory address
 * @param device The given I2C interface
 * @param mem_addr The memory address that's going to be read
 * @param mem_size The size of the memory address that's going to be read
 * @param rx_buffer A pointer to the data buffer containing the data transmitted
 * from the device connected to the I2C interface.
 * @param rx_buffer_size The size of the rx_data buffer.
 * @return True if data is transmitted successfully, false otherwise
 */
ExitCode hw_i2c_memoryRead(const I2cDevice *device, uint16_t mem_addr, uint8_t *rx_buffer, uint16_t rx_buffer_size);

/**
 * @brief Write an amount of data to a specific memory address
 * @param device The given I2C interface
 * @param mem_addr The memory address that's going to be read
 * @param tx_buffer A pointer to the data buffer containing the data transmitted
 * to the device connected to the I2C interface.
 * @param tx_buffer_size The size fo the tx_buffer
 * @return True if data is read successfully, false otherwise
 */
ExitCode
    hw_i2c_memoryWrite(const I2cDevice *device, uint16_t mem_addr, const uint8_t *tx_buffer, uint16_t tx_buffer_size);
