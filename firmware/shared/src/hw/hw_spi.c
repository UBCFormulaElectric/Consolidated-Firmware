#include "hw_spi.h"
#include <assert.h>
#include <stdlib.h>

void hw_spi_setNssLow(const SpiInterface *spi)
{
    HAL_GPIO_WritePin(spi->nss_port, spi->nss_pin, GPIO_PIN_RESET);
}

void hw_spi_setNssHigh(const SpiInterface *spi)
{
    HAL_GPIO_WritePin(spi->nss_port, spi->nss_pin, GPIO_PIN_SET);
}

bool hw_spi_transmitAndReceive(
    const SpiInterface *spi,
    uint8_t            *tx_buffer,
    uint16_t            tx_buffer_size,
    uint8_t            *rx_buffer,
    uint16_t            rx_buffer_size)

{
    bool status = false;

    hw_spi_setNssLow(spi);

    if (HAL_SPI_Transmit(spi->spi_handle, tx_buffer, tx_buffer_size, spi->timeout_ms) == HAL_OK)
    {
        status = HAL_SPI_Receive(spi->spi_handle, rx_buffer, rx_buffer_size, spi->timeout_ms) == HAL_OK;
    }

    hw_spi_setNssHigh(spi);

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
