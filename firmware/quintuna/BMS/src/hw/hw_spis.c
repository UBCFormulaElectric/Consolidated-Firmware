#include "hw_spis.h"

#define LTC6813_SPI_TIMEOUT_MS (10U)

static const SpiInterface ltc6813_spi_ls = { .spi_handle = &hspi4,
    .nss_port   = spi_cs_ls_pin->port,
    .nss_pin    = spi_cs_ls_pin->pin,
    .timeout_ms = LTC6813_SPI_TIMEOUT_MS };

static const SpiInterface ltc6813_spi_hs = { .spi_handle = &hspi4,
    .nss_port   = spi_cs_hs_pin->port,
    .nss_pin    = spi_cs_hs_pin->pin,
    .timeout_ms = LTC6813_SPI_TIMEOUT_MS };
