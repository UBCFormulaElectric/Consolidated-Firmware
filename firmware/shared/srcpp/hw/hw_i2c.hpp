#pragma once

#include <cstdint>
#include <span>
#include "main.h"
#include "hw_utils.hpp"

namespace hw::i2c
{
class I2CDevice;

class I2CBus
{
  public:
    constexpr explicit I2CBus(I2C_HandleTypeDef &handle_in) : handle(handle_in), taskInProgress(nullptr){};

    /**
     * @brief Deinitialize the I2C bus.
     */
    void deinit() const;

    /**
     * @brief Notify the task waiting on an I2C transaction from an ISR.
     * Called by HAL I2C completion callbacks to signal that a transaction has finished.
     */
    void onTransactionCompleteFromISR();

  private:
    friend class I2CDevice;

    I2C_HandleTypeDef &handle;
    TaskHandle_t       taskInProgress;
};

class I2CDevice
{
  public:
    constexpr explicit I2CDevice(I2CBus &bus_in, const uint8_t targetAddr_in, const uint32_t timeoutMs_in)
      : bus(bus_in), targetAddress(targetAddr_in), timeoutMs(timeoutMs_in){};

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
    [[nodiscard]] ExitCode receive(std::span<uint8_t> rx_buffer) const;

    /**
     * @brief Transmit data to the device connected to the given I2C interface.
     * @param tx_buffer A data buffer containing the data transmitted
     * to the device connected to the I2C interface.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] ExitCode transmit(std::span<const uint8_t> tx_buffer) const;

    /**
     * @brief Read an amount of data from a specific memory address
     * @param mem_addr The memory address that's going to be read
     * @param rx_buffer A data buffer containing the data transmitted from the device connected to the I2C interface.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] ExitCode memoryRead(uint16_t mem_addr, std::span<uint8_t> rx_buffer) const;

    /**
     * @brief Write an amount of data to a specific memory address
     * @param mem_addr The memory address that's going to be read
     * @param tx_buffer A data buffer containing the data transmitted
     * to the device connected to the I2C interface.
     * @return EXIT_CODE_OK if data is read successfully.
     */
    [[nodiscard]] ExitCode memoryWrite(uint16_t mem_addr, std::span<const uint8_t> tx_buffer) const;

  private:
    I2CBus  &bus;
    uint8_t  targetAddress;
    uint32_t timeoutMs;

    [[nodiscard]] ExitCode waitForNotification();
};

/**
 * @brief Retrieve the I2CBus instance associated with a given HAL I2C handle.
 * @param handle Pointer to a HAL I2C handle.
 * @return Reference to the I2CBus corresponding to the handle.
 */
[[nodiscard]] I2CBus &getBusFromHandle(const I2C_HandleTypeDef *handle);
} // namespace hw::i2c
