#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
extern "C"
{
#include "hw_hal.h"
}
#endif

namespace hw::i2c
{
class I2c
{
  private:
    I2C_HandleTypeDef *i2c_handle;
    uint16_t           target_address;
    uint32_t           timeout_ms;

  public:
    explicit I2c(const I2C_HandleTypeDef *i2c_handle_in, const uint16_t target_address_in, const uint32_t timeout_ms_in)
      : i2c_handle(i2c_handle_in), target_address(target_address_in), timeout_ms(timeout_ms_in)
    {
    }

    /**
     * @brief Check if device connected to the given I2C interface is ready for communication.
     * @param i2c The given I2C interface.
     * @return True if connected device is ready to communicate over I2C.
     */
    bool hw_i2c_isTargetReady();

    /**
     * @brief Receive data from the device connected to the given I2C interface.
     * @param i2c The given I2C interface.
     * @param rx_buffer A pointer to the data buffer containing the data transmitted
     * from the device connected to the I2C interface.
     * @param rx_buffer_size The size of the rx_data buffer.
     * @return True if data is transmitted successfully. Else return false.
     */
    bool hw_i2c_receive(uint8_t *rx_buffer, uint16_t rx_buffer_size);

    /**
     * @brief Transmit data to the device connected to the given I2C interface.
     * @param i2c The given I2C interface.
     * @param tx_buffer A pointer to the data buffer containing the data transmitted
     * to the device connected to the I2C interface.
     * @param tx_buffer_size The size of the tx_data buffer.
     * @return True if data is transmitted successfully. Else return false.
     */
    bool hw_i2c_transmit(uint8_t *tx_buffer, uint16_t tx_buffer_size);

    /**
     * @brief Read an amount of data from a specific memory address
     * @param i2c The given I2C interface
     * @param mem_addr The memory address that's going to be read
     * @param mem_size The size of the memory address that's going to be read
     * @param rx_buffer A pointer to the data buffer containing the data transmitted
     * from the device connected to the I2C interface.
     * @param rx_buffer_size The size of the rx_data buffer.
     * @return True if data is transmitted successfully, false otherwise
     */
    bool hw_i2c_memRead(uint16_t mem_addr, uint8_t *rx_buffer, uint16_t rx_buffer_size);

    /**
     * @brief Write an amount of data to a specific memory address
     * @param i2c The given I2C interface
     * @param mem_addr The memory address that's going to be read
     * @param tx_buffer A pointer to the data buffer containing the data transmitted
     * to the device connected to the I2C interface.
     * @param tx_buffer_size The size fo the tx_buffer
     * @return True if data is read successfully, false otherwise
     */
    bool hw_i2c_memWrite(uint16_t mem_addr, uint8_t *tx_buffer, uint16_t tx_buffer_size);
};
} // namespace hw::i2c
