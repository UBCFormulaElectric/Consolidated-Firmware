#include <assert.h>
#include <stdlib.h>
#include "Io_SharedSpi.h"

struct SharedSpi
{
    SPI_HandleTypeDef *spi_handle;
    GPIO_TypeDef *     nss_port;
    uint16_t           nss_pin;
    uint32_t           timeout_ms;
};

struct SharedSpi *
    Io_SharedSpi_Create(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *nss_port, uint16_t nss_pin, uint32_t timeout_ms)
{
    assert(spi_handle != NULL);

    struct SharedSpi *spi_interface = malloc(sizeof(struct SharedSpi));
    assert(spi_interface != NULL);

    spi_interface->spi_handle = spi_handle;
    spi_interface->nss_pin    = nss_pin;
    spi_interface->nss_port   = nss_port;
    spi_interface->timeout_ms = timeout_ms;

    return spi_interface;
}

void Io_SharedSpi_SetNssLow(const struct SharedSpi *const spi_interface)
{
    HAL_GPIO_WritePin(spi_interface->nss_port, spi_interface->nss_pin, GPIO_PIN_RESET);
}

void Io_SharedSpi_SetNssHigh(const struct SharedSpi *const spi_interface)
{
    HAL_GPIO_WritePin(spi_interface->nss_port, spi_interface->nss_pin, GPIO_PIN_SET);
}

bool Io_SharedSpi_TransmitAndReceive(
    const struct SharedSpi *const spi_interface,
    uint8_t *                     tx_buffer,
    uint16_t                      tx_buffer_size,
    uint8_t *                     rx_buffer,
    uint16_t                      rx_buffer_size)

{
    bool status = false;

    Io_SharedSpi_SetNssLow(spi_interface);

    if (HAL_SPI_Transmit(spi_interface->spi_handle, tx_buffer, tx_buffer_size, spi_interface->timeout_ms) == HAL_OK)
    {
        status =
            HAL_SPI_Receive(spi_interface->spi_handle, rx_buffer, rx_buffer_size, spi_interface->timeout_ms) == HAL_OK;
    }

    Io_SharedSpi_SetNssHigh(spi_interface);

    return status;
}

bool Io_SharedSpi_Transmit(const struct SharedSpi *const spi_interface, uint8_t *tx_buffer, uint16_t tx_buffer_size)
{
    Io_SharedSpi_SetNssLow(spi_interface);
    const bool status =
        HAL_SPI_Transmit(spi_interface->spi_handle, tx_buffer, tx_buffer_size, spi_interface->timeout_ms) == HAL_OK;
    Io_SharedSpi_SetNssHigh(spi_interface);

    return status;
}

bool Io_SharedSpi_Receive(const struct SharedSpi *const spi_interface, uint8_t *rx_buffer, uint16_t rx_buffer_size)
{
    Io_SharedSpi_SetNssLow(spi_interface);
    const bool status =
        HAL_SPI_Receive(spi_interface->spi_handle, rx_buffer, rx_buffer_size, spi_interface->timeout_ms) == HAL_OK;
    Io_SharedSpi_SetNssHigh(spi_interface);

    return status;
}

bool Io_SharedSpi_MultipleTransmitWithoutNssToggle(
    const struct SharedSpi *const spi_interface,
    uint8_t *                     tx_buffer,
    uint16_t                      tx_buffer_size,
    size_t                        num_tx_data_copies)
{
    for (size_t i = 0U; i < num_tx_data_copies; i++)
    {
        if (HAL_SPI_Transmit(spi_interface->spi_handle, tx_buffer, tx_buffer_size, spi_interface->timeout_ms) != HAL_OK)
        {
            return false;
        }
    }

    return true;
}

bool Io_SharedSpi_TransmitWithoutNssToggle(
    const struct SharedSpi *const spi_interface,
    uint8_t *                     tx_data,
    uint16_t                      tx_buffer_size)
{
    return HAL_SPI_Transmit(spi_interface->spi_handle, tx_data, tx_buffer_size, spi_interface->timeout_ms) == HAL_OK;
}
