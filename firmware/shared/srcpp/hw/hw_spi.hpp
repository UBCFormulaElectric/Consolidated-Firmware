#pragma once

extern "C"
{
#include "main.h"
#ifndef HAL_SPI_MODULE_ENABLED
#error "HAL_SPI_MODULE_ENABLED must be defined and set to 1"
#endif
}

#include <span>
#include "FreeRTOS.h"
#include "task.h"
#include "util_errorCodes.hpp"
#include "hw_gpio.hpp"

namespace hw::spi
{
class SpiDevice;

class SpiBus
{
  public:
    constexpr explicit SpiBus(SPI_HandleTypeDef &handle_in) : handle(handle_in) {}
    SPI_HandleTypeDef &handle;

    /**
     * @brief Deinitialize the SPI bus.
     */
    void deinit() const;

    /**
     * @brief Notify the task waiting on an SPI transaction from an ISR context.
     */
    void onTransactionCompleteFromISR() const;

  private:
    friend class SpiDevice;

    mutable TaskHandle_t taskInProgress{ nullptr }; // Task currently performing a transaction.
};

class SpiDevice
{
  public:
    constexpr SpiDevice(const SpiBus &bus_in, const Gpio &nss_in, const uint32_t timeoutMs_in)
      : bus(bus_in), nss(nss_in), timeoutMs(timeoutMs_in)
    {
    }

    /**
     * @brief Transmit data to the SPI device.
     * @param tx Buffer containing the data to transmit.
     * @return EXIT_CODE_OK if transmission succeeded, otherwise an error code.
     */
    [[nodiscard]] std::expected<void, ErrorCode> transmit(std::span<const uint8_t> tx) const;

    /**
     * @brief Receive data from the SPI device.
     * @param rx Buffer to store received data.
     * @return EXIT_CODE_OK if reception succeeded, otherwise an error code.
     */
    [[nodiscard]] std::expected<void, ErrorCode> receive(std::span<uint8_t> rx) const;

    /**
     * @brief Transmit and then receive data over SPI while keeping NSS asserted.
     *
     * @param tx Buffer containing data to transmit first.
     * @param rx Buffer to store received data after transmission.
     * @return EXIT_CODE_OK if the operation succeeded, otherwise an error code.
     */
    [[nodiscard]] std::expected<void, ErrorCode>
        transmitThenReceive(std::span<const uint8_t> tx, std::span<uint8_t> rx) const;

  private:
    const SpiBus &bus;
    const Gpio   &nss;
    uint32_t      timeoutMs;

    void enableNss() const;
    void disableNss() const;

    /**
     * @return idk
     */
    [[nodiscard]] std::expected<void, ErrorCode> waitForNotification() const;
};

/**
 * @brief Retrieve the SpiBus instance associated with a given HAL SPI handle.
 * @param handle Pointer to the HAL SPI handle.
 * @return Reference to the associated SpiBus object.
 */
[[nodiscard]] const SpiBus &getBusFromHandle(const SPI_HandleTypeDef *handle);

} // namespace hw::spi
