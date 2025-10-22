#pragma once

#include <cstdint>
#include <map>
#include <span>
#include "main.h"
#include "hw_utils.hpp"

namespace hw::i2c
{
class I2CDevice;

class I2CBus
{
  public:
    explicit I2CBus(I2C_HandleTypeDef *handle);
    void                         deinit() const;
    [[nodiscard]] static I2CBus &getBusFromHandle(const I2C_HandleTypeDef *handle);
    void                         onTransactionCompleteFromISR();

  private:
    friend class I2CDevice;

    I2C_HandleTypeDef *handle_;
    TaskHandle_t       taskInProgress_;
};

class I2CDevice
{
  public:
    explicit I2CDevice(I2CBus &bus, uint8_t targetAddr, uint32_t timeoutMs);

    /**
     * @brief Check if device connected to the given I2C interface is ready for communication.
     * @return EXIT_CODE_OK if connected device is ready to communicate over I2C.
     */
    [[nodiscard]] ExitCode isTargetReady() const;

    /**
     * @brief Receive data from the device connected to the given I2C interface.
     * @param rx_buffer A data buffer containing the data transmitted
     * from the device connected to the I2C interface.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] ExitCode receive(std::span<uint8_t> rxBuffer);

    /**
     * @brief Transmit data to the device connected to the given I2C interface.
     * @param tx_buffer A data buffer containing the data transmitted
     * to the device connected to the I2C interface.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] ExitCode transmit(std::span<const uint8_t> txBuffer);

    /**
     * @brief Read an amount of data from a specific memory address
     * @param mem_addr The memory address that's going to be read
     * @param rx_buffer A data buffer containing the data transmitted
     * from the device connected to the I2C interface.
     * @param rx_buffer_size The size of the rx_data buffer.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] ExitCode memoryRead(uint16_t memAddr, std::span<uint8_t> rxBuffer);

    /**
     * @brief Write an amount of data to a specific memory address
     * @param mem_addr The memory address that's going to be read
     * @param tx_buffer A data buffer containing the data transmitted
     * to the device connected to the I2C interface.
     * @return EXIT_CODE_OK if data is read successfully.
     */
    [[nodiscard]] ExitCode memoryWrite(uint16_t memAddr, std::span<const uint8_t> txBuffer);

  private:
    I2CBus  &bus_;
    uint8_t  targetAddress_;
    uint32_t timeoutMs_;

    [[nodiscard]] ExitCode waitForNotification();
};
} // namespace hw::i2c
