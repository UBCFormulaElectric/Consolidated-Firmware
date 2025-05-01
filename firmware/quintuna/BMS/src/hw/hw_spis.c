#include "hw_spis.h"

#include <assert.h>

#define LTC6813_SPI_TIMEOUT_MS (100U)

static SpiBus isospi_bus = { .handle = &hspi4 };

const SpiDevice ltc6813_spi_ls = { .bus        = &isospi_bus,
                                   .nss_pin    = &spi_cs_ls_pin,
                                   .timeout_ms = LTC6813_SPI_TIMEOUT_MS };

const SpiDevice ltc6813_spi_hs = { .bus        = &isospi_bus,
                                   .nss_pin    = &spi_cs_hs_pin,
                                   .timeout_ms = LTC6813_SPI_TIMEOUT_MS };

SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == isospi_bus.handle);
    return &isospi_bus;
}