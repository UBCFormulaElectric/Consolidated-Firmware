#include "hw_spi.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void hw_spi_setNssLow(const SpiInterface *spi)
{
    HAL_GPIO_WritePin(spi->nss_port, spi->nss_pin, GPIO_PIN_RESET);
}

void hw_spi_setNssHigh(const SpiInterface *spi)
{
    HAL_GPIO_WritePin(spi->nss_port, spi->nss_pin, GPIO_PIN_SET);
}

bool hw_spi_transmitThenReceive(
    const SpiInterface *spi,
    uint8_t            *tx_buffer,
    uint16_t            tx_buffer_size,
    uint8_t            *rx_buffer,
    uint16_t            rx_buffer_size)

{
    bool status = false;

    const uint16_t combined_size = tx_buffer_size + rx_buffer_size;

    // HAL_SPI_TransmitRecieve requires tx buffer and rx buffer to be of size equal to number of bytes to transmit and
    // recieve
    uint8_t padded_tx_buffer[combined_size];
    uint8_t padded_rx_buffer[combined_size];

    // Copy tx_buffer into beginning of larger padded_tx_buffer
    memcpy(padded_tx_buffer, tx_buffer, tx_buffer_size);

    hw_spi_setNssLow(spi);

    status = HAL_SPI_TransmitReceive(
                 spi->spi_handle, padded_tx_buffer, padded_rx_buffer, combined_size, spi->timeout_ms) == HAL_OK;

    hw_spi_setNssHigh(spi);

    /* Data will not be returned over SPI until command has finished, so data in first tx_buffer_size bytes not relevant
    Copy entries at the end of padded_rx_buffer back into rx_buffer */

    memcpy(rx_buffer, &padded_rx_buffer[tx_buffer_size], rx_buffer_size);

    return status;
}

bool hw_spi_transmit(const SpiInterface *spi, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    hw_spi_setNssLow(spi);
    const bool status = HAL_SPI_Transmit(spi->spi_handle, tx_buffer, tx_buffer_size, spi->timeout_ms) == HAL_OK;
    hw_spi_setNssHigh(spi);

    return status;
}

bool hw_spi_receive(const SpiInterface *spi, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    hw_spi_setNssLow(spi);
    const bool status = HAL_SPI_Receive(spi->spi_handle, rx_buffer, rx_buffer_size, spi->timeout_ms) == HAL_OK;
    hw_spi_setNssHigh(spi);

    return status;
}

bool hw_spi_transmitWithoutNssToggle(const SpiInterface *spi, uint8_t *tx_data, uint16_t tx_buffer_size)
{
    return HAL_SPI_Transmit(spi->spi_handle, tx_data, tx_buffer_size, spi->timeout_ms) == HAL_OK;
}
