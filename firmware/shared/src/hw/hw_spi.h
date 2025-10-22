#pragma once

#include "hw_gpio.h"
#include "main.h"
#include "errorCodes.h"

typedef struct
{
    SPI_HandleTypeDef *const handle;
    TaskHandle_t             task_in_progress;
} SpiBus;

typedef struct
{
    SpiBus     *bus;
    const Gpio *nss_pin;
    uint32_t    timeout_ms;
} SpiDevice;

// Board-specific config: You need to define this function which maps STM32 handle to SpiBus struct!
SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle);

/**
 * Transmit data to and receive data from the device connected to the given SPI
 * interface.
 * @param device The given SPI device.
 * @param tx_buffer A pointer to the data buffer containing the data transmitted
 * to the device connected to the SPI interface.
 * @param tx_buffer_size The size of the tx_data buffer.
 * @param rx_buffer A pointer to the data buffer that stores the data received
 * from the device connected to the SPI interface.
 * @param rx_buffer_size The number data received from the device connected to
 * the SPI interface.
 * @return True if data is transmitted and received successfully. Else return
 * false.
 */
ExitCode hw_spi_transmitThenReceive(
    const SpiDevice *device,
    const uint8_t   *tx_buffer,
    uint16_t         tx_buffer_size,
    uint8_t         *rx_buffer,
    uint16_t         rx_buffer_size);

/**
 * Transmit data to the device connected to the given SPI interface.
 * @param device The given SPI interface.
 * @param tx_buffer A pointer to the data buffer containing the data transmitted
 * to the device connected to the SPI interface.
 * @param tx_buffer_size The size of the tx_data buffer.
 * @return True if data is transmitted successfully. Else, return false.
 */
ExitCode hw_spi_transmit(const SpiDevice *device, const uint8_t *tx_buffer, uint16_t tx_buffer_size);

/**
 * Receive data from the device connected to the given SPI interface.
 * @param device The given SPI interface.
 * @param rx_buffer A pointer to the data buffer that stores the data received
 * from the device connected to the SPI interface.
 * @param rx_buffer_size The size of the rx_data buffer.
 * @return True if data is received successfully. Else, return false.
 */
ExitCode hw_spi_receive(const SpiDevice *device, uint8_t *rx_buffer, uint16_t rx_buffer_size);
