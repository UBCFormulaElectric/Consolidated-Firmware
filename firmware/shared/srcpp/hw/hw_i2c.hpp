#pragma once

#include <span>
#include "hw_utils.hpp"
#include "FreeRTOS.h"
#include "task.h"

namespace hw::i2c
{
class device;

class bus
{
  public:
    consteval explicit bus(I2C_HandleTypeDef &handle_in) : handle(handle_in), taskInProgress(nullptr) {}

    /**
     * @brief Deinitialize the I2C bus.
     */
    void deinit() const { HAL_I2C_DeInit(&handle); }

    /**
     * @brief Notify the task waiting on an I2C transaction from an ISR.
     * Called by HAL I2C completion callbacks to signal that a transaction has finished.
     */
    void onTransactionCompleteFromISR() const;

  private:
    friend class device;

    I2C_HandleTypeDef   &handle;
    mutable TaskHandle_t taskInProgress;
};

class device
{
    const bus     &d_bus;
    const uint8_t  targetAddress;
    const uint32_t timeoutMs;

    [[nodiscard]] std::expected<void, ErrorCode> waitForNotification() const;

  public:
    consteval explicit device(const i2c::bus &bus_in, const uint8_t targetAddr_in, const uint32_t timeoutMs_in)
      : d_bus(bus_in), targetAddress(targetAddr_in), timeoutMs(timeoutMs_in){};

    static constexpr uint8_t NUM_DEVICE_READY_TRIALS = 5;
    /**
     * @brief Check if device connected to the given I2C interface is ready for communication.
     * @return EXIT_CODE_OK if connected device is ready to communicate over I2C.
     */
    [[nodiscard]] std::expected<void, ErrorCode> isTargetReady() const
    {
        return utils::convertHalStatus(HAL_I2C_IsDeviceReady(
            &d_bus.handle, static_cast<uint16_t>(targetAddress << 1), NUM_DEVICE_READY_TRIALS, timeoutMs));
    }

    /**
     * @brief Receive data from the device connected to the given I2C interface.
     * @param rx_buffer A data buffer containing the data transmitted
     * from the device connected to the I2C interface.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] std::expected<void, ErrorCode> receive(std::span<uint8_t> rx_buffer) const;

    /**
     * @brief Transmit data to the device connected to the given I2C interface.
     * @param tx_buffer A data buffer containing the data transmitted
     * to the device connected to the I2C interface.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] std::expected<void, ErrorCode> transmit(std::span<const uint8_t> tx_buffer) const;

    /**
     * @brief Read an amount of data from a specific memory address
     * @param mem_addr The memory address that's going to be read
     * @param rx_buffer A data buffer containing the data transmitted from the device connected to the I2C interface.
     * @return EXIT_CODE_OK if data is transmitted successfully.
     */
    [[nodiscard]] std::expected<void, ErrorCode> memoryRead(uint16_t mem_addr, std::span<uint8_t> rx_buffer) const;

    /**
     * @brief Write an amount of data to a specific memory address
     * @param mem_addr The memory address that's going to be read
     * @param tx_buffer A data buffer containing the data transmitted
     * to the device connected to the I2C interface.
     * @return EXIT_CODE_OK if data is read successfully.
     */
    [[nodiscard]] std::expected<void, ErrorCode>
        memoryWrite(uint16_t mem_addr, std::span<const uint8_t> tx_buffer) const;
};

/**
 * @brief Retrieve the I2CBus instance associated with a given HAL I2C handle.
 * @param handle Pointer to a HAL I2C handle.
 * @return Reference to the I2CBus corresponding to the handle.
 */
[[nodiscard]] const bus &getBusFromHandle(const I2C_HandleTypeDef *handle);
} // namespace hw::i2c
