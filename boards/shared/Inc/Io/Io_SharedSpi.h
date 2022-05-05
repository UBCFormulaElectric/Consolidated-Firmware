#pragma once

#include <stdbool.h>
#include <stm32f3xx_hal.h>
#include <stm32f3xx_hal_spi.h>

struct SharedSpi;

/**
 * Allocate and initialize a SPI interface.
 * @param spi_handle The handle of the given SPI interface.
 * @param nss_port Pointer to the NSS GPIO port of the given SPI interface.
 * @param nss_pin The NSS GPIO pin of the given SPI interface.
 * @param timeout_ms The timeout duration in milliseconds during a SPI
 * transaction which a blocking SPI transaction should wait until an error is
 * returned.
 *
 * @note NSS indicates an active low slave select for the device connected to
 * the SPI interface.
 *
 * @return A pointer to the allocated and initialized SPI interface.
 */
struct SharedSpi *
    Io_SharedSpi_Create(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *nss_port, uint16_t nss_pin, uint32_t timeout_ms);

/**
 * Set the NSS pin low for the given SPI interface.
 * @param spi_interface The given SPI interface.
 */
void Io_SharedSpi_SetNssLow(const struct SharedSpi *spi_interface);

/**
 * Set the NSS pin high for the given SPI interface.
 * @param spi_interface The given SPI interface.
 */
void Io_SharedSpi_SetNssHigh(const struct SharedSpi *spi_interface);

/**
 * Transmit data to and receive data from the device connected to the given SPI
 * interface.
 * @param spi_interface The given SPI interface.
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
bool Io_SharedSpi_TransmitAndReceive(
    const struct SharedSpi *spi_interface,
    uint8_t *               tx_buffer,
    uint16_t                tx_buffer_size,
    uint8_t *               rx_buffer,
    uint16_t                rx_buffer_size);

/**
 * Transmit data to the device connected to the given SPI interface.
 * @param spi_interface The given SPI interface.
 * @param tx_buffer A pointer to the data buffer containing the data transmitted
 * to the device connected to the SPI interface.
 * @param tx_buffer_size The size of the tx_data buffer.
 * @return True if data is transmitted successfully. Else, return false.
 */
bool Io_SharedSpi_Transmit(const struct SharedSpi *spi_interface, uint8_t *tx_buffer, uint16_t tx_buffer_size);

/**
 * Receive data from the device connected to the given SPI interface.
 * @param spi_interface The given SPI interface.
 * @param rx_buffer A pointer to the data buffer that stores the data received
 * from the device connected to the SPI interface.
 * @param rx_buffer_size The size of the rx_data buffer.
 * @return True if data is received successfully. Else, return false.
 */
bool Io_SharedSpi_Receive(const struct SharedSpi *spi_interface, uint8_t *rx_buffer, uint16_t rx_buffer_size);

/**
 * Transmit multiple copies of the a data packet to the device connected to the
 * given SPI interface without toggling the NSS pin.
 * @param spi_interface The given SPI interface.
 * @param tx_buffer A pointer to the data buffer containing the data transmitted
 * to the device connected to the SPI interface.
 * @param tx_buffer_size The size of the tx_data buffer.
 * @param num_tx_data_copies The number of copies of tx_data transmitted to the
 * device connected to the SPI interface.
 * @return True if data is transmitted successfully. Else, return false.
 */
bool Io_SharedSpi_MultipleTransmitWithoutNssToggle(
    const struct SharedSpi *spi_interface,
    uint8_t *               tx_buffer,
    uint16_t                tx_buffer_size,
    size_t                  num_tx_data_copies);

/**
 * Transmit data to the device connected to the given SPI interface without
 * toggling the NSS pin.
 * @param spi_interface The given SPI interface.
 * @param tx_data A pointer to the data buffer containing the data transmitted
 * to the device connected to the SPI interface.
 * @param tx_buffer_size The size of the tx_data buffer.
 * @return True if data is transmitted successfully. Else, return false.
 */
bool Io_SharedSpi_TransmitWithoutNssToggle(
    const struct SharedSpi *spi_interface,
    uint8_t *               tx_buffer,
    uint16_t                tx_buffer_size);
