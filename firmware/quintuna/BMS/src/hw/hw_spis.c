#include "hw_spis.h"

#define LTC6813_SPI_TIMEOUT_MS (10U)

static SpiBus isospi_bus = { .handle = &hspi4 };

const SpiDevice ltc6813_spi_ls = { .bus        = &isospi_bus,
                                   .nss_pin    = &spi_cs_ls_pin,
                                   .timeout_ms = LTC6813_SPI_TIMEOUT_MS };

const SpiDevice ltc6813_spi_hs = { .bus        = &isospi_bus,
                                   .nss_pin    = &spi_cs_hs_pin,
                                   .timeout_ms = LTC6813_SPI_TIMEOUT_MS };
