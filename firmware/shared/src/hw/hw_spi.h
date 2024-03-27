#pragma once

#include <stdbool.h>
#include "hw_hal.h"

typedef struct
{
    SPI_HandleTypeDef *spi_handle;
    GPIO_TypeDef      *nss_port;
    uint16_t           nss_pin;
    uint32_t           timeout_ms;
} SpiInterface;

/**
 * Set the NSS pin low for the given SPI interface.
 * @param spi The given SPI interface.
 */
void hw_spi_setNssLow(const SpiInterface *spi);

/**
 * Set the NSS pin high for the given SPI interface.
 * @param spi The given SPI interface.
 */
void hw_spi_setNssHigh(const SpiInterface *spi);

/**
 * Transmit data to and receive data from the device connected to the given SPI
 * interface.
 * @param spi The given SPI interface.
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
bool hw_spi_transmitAndReceive(
    const SpiInterface *spi,
    uint8_t            *tx_buffer,
    uint16_t            tx_buffer_size,
    uint8_t            *rx_buffer,
    uint16_t            rx_buffer_size);

/**
 * Transmit data to the device connected to the given SPI interface.
 * @param spi The given SPI interface.
 * @param tx_buffer A pointer to the data buffer containing the data transmitted
 * to the device connected to the SPI interface.
 * @param tx_buffer_size The size of the tx_data buffer.
 * @return True if data is transmitted successfully. Else, return false.
 */
bool hw_spi_transmit(const SpiInterface *spi, uint8_t *tx_buffer, uint16_t tx_buffer_size);

/**
 * Receive data from the device connected to the given SPI interface.
 * @param spi The given SPI interface.
 * @param rx_buffer A pointer to the data buffer that stores the data received
 * from the device connected to the SPI interface.
 * @param rx_buffer_size The size of the rx_data buffer.
 * @return True if data is received successfully. Else, return false.
 */
bool hw_spi_receive(const SpiInterface *spi, uint8_t *rx_buffer, uint16_t rx_buffer_size);

/**
 * Transmit data to the device connected to the given SPI interface without
 * toggling the NSS pin.
 * @param spi The given SPI interface.
 * @param tx_data A pointer to the data buffer containing the data transmitted
 * to the device connected to the SPI interface.
 * @param tx_buffer_size The size of the tx_data buffer.
 * @return True if data is transmitted successfully. Else, return false.
 */
bool hw_spi_transmitWithoutNssToggle(const SpiInterface *spi, uint8_t *tx_buffer, uint16_t tx_buffer_size);
