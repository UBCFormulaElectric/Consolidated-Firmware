#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "hw_gpio.h"
#include "main.h"

typedef struct
{
    uint32_t   spi_bus;
    Gpio     nss_pin;
    uint32_t timeout_ms;
} SpiDevice;

// Board-specific config. Must define:
// 1. SpiBus: Enum of all the SPI busses.
// 2. spi_bus_handles: Map of bus enum to STM32 handle.
// 3. All the used SPI devices.
#include "hw_spis.h"

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
bool hw_spi_transmitThenReceive(
    const SpiDevice* device,
    uint8_t  *tx_buffer,
    uint16_t  tx_buffer_size,
    uint8_t  *rx_buffer,
    uint16_t  rx_buffer_size);

/**
 * Transmit data to the device connected to the given SPI interface.
 * @param device The given SPI interface.
 * @param tx_buffer A pointer to the data buffer containing the data transmitted
 * to the device connected to the SPI interface.
 * @param tx_buffer_size The size of the tx_data buffer.
 * @return True if data is transmitted successfully. Else, return false.
 */
bool hw_spi_transmit(const SpiDevice* device, uint8_t *tx_buffer, uint16_t tx_buffer_size);

/**
 * Receive data from the device connected to the given SPI interface.
 * @param device The given SPI interface.
 * @param rx_buffer A pointer to the data buffer that stores the data received
 * from the device connected to the SPI interface.
 * @param rx_buffer_size The size of the rx_data buffer.
 * @return True if data is received successfully. Else, return false.
 */
bool hw_spi_receive(const SpiDevice *device, uint8_t *rx_buffer, uint16_t rx_buffer_size);
