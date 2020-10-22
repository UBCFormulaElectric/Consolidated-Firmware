#include <assert.h>
#include <stdlib.h>
#include "Io_SharedSpi.h"

#define EXIT_IF_STATUS_NOT_OK(status, nss_port, nss_pin)          \
    if ((status) != HAL_OK)                                       \
    {                                                             \
        HAL_GPIO_WritePin((nss_port), (nss_pin), GPIO_PIN_RESET); \
        return (status);                                          \
    }

struct SharedSpi
{
    SPI_HandleTypeDef *spi_handle;
    GPIO_TypeDef *     nss_port;
    uint16_t           nss_pin;
    uint32_t           timeout_ms;
};

struct SharedSpi *Io_SharedSpi_Create(
    SPI_HandleTypeDef *spi_handle,
    GPIO_TypeDef *     nss_port,
    uint16_t           nss_pin,
    uint32_t           timeout_ms)
{
    assert(spi_handle != NULL);

    struct SharedSpi *spi = malloc(sizeof(struct SharedSpi));
    assert(spi != NULL);

    spi->spi_handle = spi_handle;
    spi->nss_pin    = nss_pin;
    spi->nss_port   = nss_port;
    spi->timeout_ms = timeout_ms;

    return spi;
}

void Io_SharedSpi_SetNssLow(const struct SharedSpi *const spi)
{
    HAL_GPIO_WritePin(spi->nss_port, spi->nss_pin, GPIO_PIN_RESET);
}

void Io_SharedSpi_SetNssHigh(const struct SharedSpi *const spi)
{
    HAL_GPIO_WritePin(spi->nss_port, spi->nss_pin, GPIO_PIN_SET);
}

HAL_StatusTypeDef Io_SharedSpi_TransmitAndReceive(
    const struct SharedSpi *const spi,
    uint8_t *                     tx_buffer,
    uint16_t                      tx_buffer_size,
    uint8_t *                     rx_buffer,
    uint16_t                      rx_buffer_size)

{
    Io_SharedSpi_SetNssLow(spi);
    EXIT_IF_STATUS_NOT_OK(
        HAL_SPI_Transmit(
            spi->spi_handle, tx_buffer, tx_buffer_size, spi->timeout_ms),
        spi->nss_port, spi->nss_pin)
    EXIT_IF_STATUS_NOT_OK(
        HAL_SPI_Receive(
            spi->spi_handle, rx_buffer, rx_buffer_size, spi->timeout_ms),
        spi->nss_port, spi->nss_pin)
    Io_SharedSpi_SetNssHigh(spi);

    return HAL_OK;
}

HAL_StatusTypeDef Io_SharedSpi_Transmit(
    const struct SharedSpi *const spi,
    uint8_t *                     tx_buffer,
    uint16_t                      tx_buffer_size)
{
    Io_SharedSpi_SetNssLow(spi);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(
        spi->spi_handle, tx_buffer, tx_buffer_size, spi->timeout_ms);
    Io_SharedSpi_SetNssHigh(spi);

    return status;
}

HAL_StatusTypeDef Io_SharedSpi_Receive(
    const struct SharedSpi *const spi,
    uint8_t *                     rx_buffer,
    uint16_t                      rx_buffer_size)
{
    Io_SharedSpi_SetNssLow(spi);
    HAL_StatusTypeDef status = HAL_SPI_Receive(
        spi->spi_handle, rx_buffer, rx_buffer_size, spi->timeout_ms);
    Io_SharedSpi_SetNssHigh(spi);

    return status;
}

HAL_StatusTypeDef Io_SharedSpi_MultipleTransmitWithoutNssToggle(
    const struct SharedSpi *const spi,
    uint8_t *                     tx_buffer,
    uint16_t                      tx_buffer_size,
    size_t                        num_tx_data_copies)
{
    HAL_StatusTypeDef status;
    for (size_t i = 0U; i < num_tx_data_copies; i++)
    {
        status = HAL_SPI_Transmit(
            spi->spi_handle, tx_buffer, tx_buffer_size, spi->timeout_ms);
        if (status != HAL_OK)
        {
            return status;
        }
    }

    return status;
}

HAL_StatusTypeDef Io_SharedSpi_TransmitWithoutNssToggle(
    const struct SharedSpi *const spi,
    uint8_t *                     tx_data,
    uint16_t                      tx_buffer_size)
{
    return HAL_SPI_Transmit(
        spi->spi_handle, tx_data, tx_buffer_size, spi->timeout_ms);
}
